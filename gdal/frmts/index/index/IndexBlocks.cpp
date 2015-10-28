#include "IndexBlocks.h"

static_assert(std::is_move_constructible<IndexBlocks>::value, "IndexBlocks should be move constructible");
static_assert(std::is_move_assignable<IndexBlocks>::value, "IndexBlocks should be move assignable");

namespace {

int getXRasterSize(const IndexLine& line) { return (line.getTileEastMax() - line.getTileEastMin()) / line.getPixelSquareSize();}
int getYRasterSize(const IndexLine& line) { return (line.getTileNorthMax() - line.getTileNorthMin()) / line.getPixelSquareSize(); }

}

IndexBlocks::IndexBlocks(const std::vector<IndexLine>& lines)
	: blockXSize(std::numeric_limits<int>::min())
	, blockYSize(std::numeric_limits<int>::min())
{
	const IndexLine* referenceLine = nullptr;

	for (const auto& line : lines)
	{
		const int xRasterSize = getXRasterSize(line);
		const int yRasterSize = getYRasterSize(line);

		const bool blockExtendsX = xRasterSize > blockXSize;
		const bool blockExtendsY = yRasterSize > blockYSize;

		const bool blockHasLessX = xRasterSize < blockXSize;
		const bool blockHasLessY = yRasterSize < blockYSize;

		if ((blockExtendsX && blockHasLessY) || (blockHasLessX && blockExtendsY))
			throw std::runtime_error("Sizes of blocks do not match up");

		if (xRasterSize >= blockXSize && yRasterSize >= blockYSize)
		{
			blockXSize = xRasterSize;
			blockYSize = yRasterSize;
			referenceLine = &line;
		}
	}

	std::vector<std::pair<const IndexLine&, IndexBlock>> unsortedBlocks;

	for (const auto& line : lines)
	{
		const auto& file = line.getTilePath();
		const int xRasterSize = getXRasterSize(line);
		const int yRasterSize = getYRasterSize(line);

		auto offsetInBlockX = line.getTileEastMin() < referenceLine->getTileEastMin() ? blockXSize - xRasterSize : 0;
		auto offsetInBlockY = line.getTileNorthMin() > referenceLine->getTileNorthMin() ? blockYSize - yRasterSize : 0;

		unsortedBlocks.emplace_back(line, IndexBlock(xRasterSize, yRasterSize, offsetInBlockX, offsetInBlockY, file));
	}
	
	for (const auto& lineWithBlock : unsortedBlocks)
	{
		const auto& line = lineWithBlock.first;
		const auto& block = lineWithBlock.second;

		auto xblockIndex = (line.getTileEastMin() - referenceLine->getTileEastMin()) / (blockXSize * referenceLine->getPixelSquareSize());
		if (xblockIndex <= 0 && (line.getTileEastMax() - line.getTileEastMin()) / referenceLine->getPixelSquareSize() < blockXSize)
			--xblockIndex;

		auto& blocksWithSameMinEasting = blocks[xblockIndex];

		auto yblockIndex = ((line.getTileNorthMin() - referenceLine->getTileNorthMin()) / (blockYSize * referenceLine->getPixelSquareSize()));
		
		//partial block below reference line
		if (yblockIndex <= 0 && (line.getTileNorthMax() - line.getTileNorthMin()) / referenceLine->getPixelSquareSize() < blockYSize)
			--yblockIndex;

		const auto ret = blocksWithSameMinEasting.insert(std::make_pair(-yblockIndex, block)); //negative y block index, because gdal starts in the upper left corner

		if (!ret.second)
			throw std::logic_error("Multiple tiles at the same coordinates");
	}

	if(!lines.empty())
	{
		static const std::int16_t ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_HEIGHTS = -3624; //swapped bytes of -9999
		
		undefBlockline.resize(blockXSize, ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_HEIGHTS);
	}

	setNrOfBlocks();
}

boost::optional<const IndexBlock&> IndexBlocks::getBlock(int blockXOffset, int blockYOffset) const
{
	if(blocks.empty())
		return {};

	auto minEast = blocks.cbegin()->first;
	auto requestedEastIndex = blockXOffset + minEast;

	auto eastIt = blocks.find(requestedEastIndex);
	if(eastIt == blocks.cend())
		return {};

	const auto& blocksWithTargetEasting = eastIt->second;
	auto minNorth = blocksWithTargetEasting.cbegin()->first; //TODO: ensure that minNorth is the same across all or store min during load
	auto requestedNorthIndex = blockYOffset + minNorth;

	auto northIt = blocksWithTargetEasting.find(requestedNorthIndex);	
	if(northIt == eastIt->second.cend())
		return {};

	return northIt->second;
}

int IndexBlocks::getBlockXSize() const
{
	return blockXSize;
}

int IndexBlocks::getBlockYSize() const
{
	return blockYSize;
}

size_t IndexBlocks::getNrBlocksX() const
{
	return nrBlocksX;
}

size_t IndexBlocks::getNrBlocksY() const
{
	return nrBlocksY;
}

void IndexBlocks::setNrOfBlocks()
{
	if(blocks.empty())
	{
		nrBlocksX = 0;
		nrBlocksY = 0;

		return;
	}

	nrBlocksX = blocks.crbegin()->first - blocks.cbegin()->first + 1;
	
	int minBlockIndex = std::numeric_limits<int>::max();
	int maxBlockIndex = std::numeric_limits<int>::min();

	for(const auto& blocksWithSameEasting : blocks)
	{
		assert(!blocksWithSameEasting.second.empty());

		minBlockIndex = std::min(minBlockIndex, blocksWithSameEasting.second.cbegin()->first);
		maxBlockIndex = std::max(maxBlockIndex, blocksWithSameEasting.second.crbegin()->first);
	}

	nrBlocksY = maxBlockIndex - minBlockIndex + 1;
}
