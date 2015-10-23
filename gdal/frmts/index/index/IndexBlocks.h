#pragma once

#include <map>
#include <vector>

#include <boost/optional/optional.hpp>

#include "IndexLine.h"

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
 	std::map<int, std::map<int, IndexBlock>> blocks;
	int blockXSize;
	int blockYSize;

public:
	IndexBlocks(const std::vector<IndexLine>& lines);

	boost::optional<const IndexBlock&> getBlock(int blockXOffset, int blockYOffset) const;

	int getBlockXSize() const;
	int getBlockYSize() const;
};

