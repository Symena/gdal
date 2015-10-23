#include "IndexBlocks.h"

static_assert(std::is_move_constructible<IndexBlocks>::value, "IndexBlocks should be move constructible");

IndexBlocks::IndexBlocks(const std::vector<IndexLine>& lines)
	: blockXSize(std::numeric_limits<int>::min())
	, blockYSize(std::numeric_limits<int>::min())
{
	for(const auto& line : lines)
	{
		const auto& file = line.getTilePath();
		const int xRasterSize = (line.getTileEastMax() - line.getTileEastMin()) / line.getPixelSquareSize();
		const int yRasterSize = (line.getTileNorthMax() - line.getTileNorthMin()) / line.getPixelSquareSize();

		auto& blocksWithSameMinEasting = blocks[line.getTileEastMin()];

		//negative northing, because gdal starts in the upper left corner
		const auto ret = blocksWithSameMinEasting.insert(std::make_pair(-line.getTileNorthMin(), IndexBlock(xRasterSize, yRasterSize, file)));

		if(!ret.second)
			throw std::runtime_error("Multiple tiles at the same coordinates");

		blockXSize = std::max(blockXSize, xRasterSize);
		blockYSize = std::max(blockYSize, yRasterSize);
	}
}

boost::optional<const IndexBlock&> IndexBlocks::getBlock(int blockXOffset, int blockYOffset) const
{
	auto eastIt = blocks.cbegin();
	std::advance(eastIt, blockXOffset);

	if(eastIt == blocks.cend())
		return {};

	auto northIt = eastIt->second.cbegin();
	std::advance(northIt, blockYOffset);
	
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
