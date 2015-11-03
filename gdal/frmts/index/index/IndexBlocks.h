#pragma once

#include <vector>

#include <boost/geometry/index/rtree.hpp>

#include "IndexLine.h"
#include "IndexGeometry.h"

class IndexBlock
{
	int rasterSizeX;
	int rasterSizeY;

	boost::filesystem::path file;

public:
	IndexBlock(int rasterSizeX, int rasterSizeY, const boost::filesystem::path& file)
		: rasterSizeX(rasterSizeX)
		, rasterSizeY(rasterSizeY)
		, file(file)
	{}

	int getRasterSizeX() const { return rasterSizeX; }
	int getRasterSizeY() const { return rasterSizeY; }

	const boost::filesystem::path& getFile() const { return file; }
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

