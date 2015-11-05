#pragma once

#include <vector>
#include <memory>

#include <boost/geometry/index/rtree.hpp>

#include "IndexLine.h"
#include "IndexGeometry.h"
#include "IndexStreamSource.h"

class IndexBlock
{
	int rasterSizeX;
	int rasterSizeY;

	std::shared_ptr<IndexStreamSource> dataStream;

	int index;

public:
	IndexBlock(int rasterSizeX, int rasterSizeY, std::shared_ptr<IndexStreamSource> dataStream, int index)
		: rasterSizeX(rasterSizeX)
		, rasterSizeY(rasterSizeY)
		, dataStream(std::move(dataStream))
		, index(index)
	{}

	int getRasterSizeX() const { return rasterSizeX; }
	int getRasterSizeY() const { return rasterSizeY; }

	int getIndex() const { return index; }

	std::unique_ptr<std::istream> getData(IndexWarnings& warnings) const {return dataStream->getStream(warnings);}
};

class IndexBlocks
{
public:
	using MapTile = std::pair<MapBox, IndexBlock>;
private:
	int blockXSize;
	int blockYSize;

	size_t nrBlocksX;
	size_t nrBlocksY;

	int pixelSquareSize;

	boost::geometry::index::rtree<MapTile, boost::geometry::index::rstar<16>> blockIndex;
	MapBox boundingBox;

public:
	explicit IndexBlocks(const std::vector<IndexLine>& lines);

	void initializeBlockIndex(const std::vector<IndexLine>& lines, const IndexLine*& referenceLine);


	MapBox getBlockBox(int blockXOffset, int blockYOffset) const;
	std::vector<MapTile> getIntersectingMapTiles(int blockXOffset, int blockYOffset) const;

	int getBlockXSize() const;
	int getBlockYSize() const;

	size_t getNrBlocksX() const;
	size_t getNrBlocksY() const;

	int getPixelSquareSize() const { return pixelSquareSize; }
	
private:
	void calculateBoundingBox(const IndexLine& referenceLine);
	void matchBoundingBoxToReferenceLine(const IndexLine& referenceLine);
	int getDistanceIncreaseForDivisibility(int xDifferenceMeter, int blockXMeter);
};

