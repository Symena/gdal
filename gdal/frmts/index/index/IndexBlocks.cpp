#include "IndexBlocks.h"

static_assert(std::is_move_constructible<IndexBlocks>::value, "IndexBlocks should be move constructible");

IndexBlocks::IndexBlocks(const std::vector<IndexLine>& lines)
	: blockXSize(std::numeric_limits<int>::min())
	, blockYSize(std::numeric_limits<int>::min())
{
	const IndexLine* referenceLine = nullptr;
	std::vector<std::pair<const IndexLine&, IndexBlock>> unsortedBlocks;

	for (const auto& line : lines)
	{
		const auto& file = line.getTilePath();
		const int xRasterSize = (line.getTileEastMax() - line.getTileEastMin()) / line.getPixelSquareSize();
		const int yRasterSize = (line.getTileNorthMax() - line.getTileNorthMin()) / line.getPixelSquareSize();

		unsortedBlocks.emplace_back(line, IndexBlock(xRasterSize, yRasterSize, file));
		
		const bool blockExtendsX = xRasterSize > blockXSize;
		const bool blockExtendsY = yRasterSize > blockYSize;

		const bool blockHasLessX = xRasterSize < blockXSize;
		const bool blockHasLessY = yRasterSize < blockYSize;

		if(xRasterSize >= blockXSize && yRasterSize >= blockYSize)
		{
			blockXSize = xRasterSize;
			blockYSize = yRasterSize;
			referenceLine = &line;
		}

		if((blockExtendsX && blockHasLessY) || (blockHasLessX && blockExtendsY))
			throw std::runtime_error("Sizes of blocks do not match up");
	}
	
	for (const auto& lineWithBlock : unsortedBlocks)
	{
		const auto& line = lineWithBlock.first;
		const auto& block = lineWithBlock.second;

		auto& blocksWithSameMinEasting = blocks[line.getTileEastMin()-referenceLine->getTileEastMin()];

		//negative northing, because gdal starts in the upper left corner
		const auto ret = blocksWithSameMinEasting.insert(std::make_pair(-(line.getTileNorthMin()-referenceLine->getTileNorthMin()), block));

		if (!ret.second)
			throw std::runtime_error("Multiple tiles at the same coordinates");
	}
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
