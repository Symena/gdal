#include "IndexBlocks.h"

#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/endian/conversion.hpp>

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

		const auto& file = line.getTilePath();

		if (xRasterSize >= blockXSize && yRasterSize >= blockYSize)
		{
			blockXSize = xRasterSize;
			blockYSize = yRasterSize;
			referenceLine = &line;
		}

		auto block = IndexBlock(xRasterSize, yRasterSize, file);

		auto lowerLeft = MapPoint(line.getTileEastMin(), line.getTileNorthMin());
		auto upperRight = MapPoint(line.getTileEastMax(), line.getTileNorthMax());

		blockIndex.insert(std::make_pair(MapBox(lowerLeft, upperRight), block));
	}

	if(!lines.empty())
	{
		static const std::int16_t ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES = boost::endian::native_to_big(static_cast<std::int16_t>(-9999));
		
		undefBlockline.resize(blockXSize, ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES);
	}

	if (referenceLine)
	{
		boundingBox = blockIndex.bounds();

		auto blockXMeter = referenceLine->getPixelSquareSize() * blockXSize;
		auto blockYMeter = referenceLine->getPixelSquareSize() * blockYSize;

		auto boxWidth = width(boundingBox);
		auto boxHeight = height(boundingBox);

		nrBlocksX = boxWidth / blockXMeter;
		nrBlocksY = boxHeight / blockYMeter;

		if (boxWidth % blockXMeter != 0)
			++nrBlocksX;

		if (boxHeight % blockYMeter != 0)
			++nrBlocksY;

		pixelSquareSize = referenceLine->getPixelSquareSize();
	}
}

MapBox IndexBlocks::getBlockBox(int blockXOffset, int blockYOffset) const
{
	int xLowerLeft = boundingBox.min_corner().get<0>() + blockXOffset * blockXSize * pixelSquareSize;
	int xUpperRight = xLowerLeft + blockXSize * pixelSquareSize;
	int yUpperRight = boundingBox.max_corner().get<1>() - blockYOffset * blockYSize * pixelSquareSize;
	int yLowerLeft = yUpperRight - blockYSize * pixelSquareSize;

	auto lowerLeft = MapPoint(xLowerLeft, yLowerLeft);
	auto upperRight = MapPoint(xUpperRight, yUpperRight);

	return MapBox(lowerLeft, upperRight);
}

std::vector<IndexBlocks::MapTile> IndexBlocks::getIntersectingMapTiles(int blockXOffset, int blockYOffset) const
{
	auto targetBox = getBlockBox(blockXOffset, blockYOffset);

	std::vector<IndexBlocks::MapTile> resultBlocks;

	auto isNotOnlyBorderMatch = [&targetBox](const MapTile& entry)
	{
		MapBox intersectionBox;
		boost::geometry::intersection(entry.first, targetBox, intersectionBox);

		auto intersectionArea = boost::geometry::area(intersectionBox);

		return intersectionArea != 0;
	};

	blockIndex.query(boost::geometry::index::intersects(targetBox)
					 && boost::geometry::index::satisfies(isNotOnlyBorderMatch), std::back_inserter(resultBlocks));

	return resultBlocks;
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
