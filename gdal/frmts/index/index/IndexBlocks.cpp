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
	: blockXSize(0)
	, blockYSize(0)
{
	const IndexLine* referenceLine = nullptr;

	initializeBlockIndex(lines, referenceLine);

	if (referenceLine)
	{
		calculateBoundingBox(*referenceLine); 

		auto boxWidth = width(boundingBox);
		auto boxHeight = height(boundingBox);

		auto blockXMeter = referenceLine->getPixelSquareSize() * blockXSize;
		auto blockYMeter = referenceLine->getPixelSquareSize() * blockYSize;

		nrBlocksX = boxWidth / blockXMeter;
		nrBlocksY = boxHeight / blockYMeter;

		if (boxWidth % blockXMeter != 0)
			++nrBlocksX;

		if (boxHeight % blockYMeter != 0)
			++nrBlocksY;

		pixelSquareSize = referenceLine->getPixelSquareSize();
	}
}

void IndexBlocks::initializeBlockIndex(const std::vector<IndexLine>& lines, const IndexLine*& referenceLine)
{
	int lineNr = 0;
	for (const auto& line : lines)
	{
		const int xRasterSize = getXRasterSize(line);
		const int yRasterSize = getYRasterSize(line);

		if (xRasterSize >= blockXSize && yRasterSize >= blockYSize)
		{
			blockXSize = xRasterSize;
			blockYSize = yRasterSize;
			referenceLine = &line;
		}

		auto block = IndexBlock(xRasterSize, yRasterSize, line.getTileDataSource(), lineNr++);

		auto lowerLeft = MapPoint(line.getTileEastMin(), line.getTileNorthMin());
		auto upperRight = MapPoint(line.getTileEastMax(), line.getTileNorthMax());

		blockIndex.insert(std::make_pair(MapBox(lowerLeft, upperRight), block));
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

void IndexBlocks::calculateBoundingBox(const IndexLine& referenceLine)
{
	boundingBox = blockIndex.bounds();

	matchBoundingBoxToReferenceLine(referenceLine);
}

int IndexBlocks::getDistanceIncreaseForDivisibility(int distance, int blockSize)
{
	int offset = 0;
	if (distance % blockSize != 0)
		offset = blockSize - (distance % blockSize);

	return offset;
}

void IndexBlocks::matchBoundingBoxToReferenceLine(const IndexLine& referenceLine)
{
	MapPoint referencePoint(referenceLine.getTileEastMin(), referenceLine.getTileNorthMax());

	const auto blockXMeter = referenceLine.getPixelSquareSize() * blockXSize;
	const auto blockYMeter = referenceLine.getPixelSquareSize() * blockYSize;

	const auto xDifferenceMeter = referencePoint.get<0>() - boundingBox.min_corner().get<0>();
	const auto yDifferenceMeter = boundingBox.max_corner().get<1>() - referencePoint.get<1>();

	const auto xOffset = getDistanceIncreaseForDivisibility(xDifferenceMeter, blockXMeter);
	const auto yOffset = getDistanceIncreaseForDivisibility(yDifferenceMeter, blockYMeter);

	const auto boundMin = boundingBox.min_corner();
	const auto boundMax = boundingBox.max_corner();

	boundingBox = MapBox({boundMin.get<0>() - xOffset, boundMin.get<1>()}, {boundMax.get<0>(), boundMax.get<1>() + yOffset});
}
