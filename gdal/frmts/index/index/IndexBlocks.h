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

public:
	IndexBlock(int rasterSizeX, int rasterSizeY, std::shared_ptr<IndexStreamSource> dataStream)
		: rasterSizeX(rasterSizeX)
		, rasterSizeY(rasterSizeY)
		, dataStream(std::move(dataStream))
	{}

	int getRasterSizeX() const { return rasterSizeX; }
	int getRasterSizeY() const { return rasterSizeY; }

	std::unique_ptr<std::istream> getData() const {return dataStream->getStream();}
};

class IndexBlocks
{
public:
	using MapTile = std::pair<MapBox, IndexBlock>;
private:
	int blockXSize;
	int blockYSize;
	std::vector<std::int16_t> undefBlockline;
	size_t nrBlocksX;
	size_t nrBlocksY;
	int pixelSquareSize;

	boost::geometry::index::rtree<MapTile, boost::geometry::index::rstar<16>> blockIndex;
	MapBox boundingBox;

public:
	explicit IndexBlocks(const std::vector<IndexLine>& lines);
	
	MapBox getBlockBox(int blockXOffset, int blockYOffset) const;
	std::vector<MapTile> getIntersectingMapTiles(int blockXOffset, int blockYOffset) const;
	const std::vector<std::int16_t>& getUndefBlockLine() const { return undefBlockline; }

	int getBlockXSize() const;
	int getBlockYSize() const;

	size_t getNrBlocksX() const;
	size_t getNrBlocksY() const;

	int getPixelSquareSize() const { return pixelSquareSize; }

};

