#pragma once

#include <map>
#include <vector>

#include <boost/optional/optional.hpp>

#include "IndexLine.h"

class IndexBlock
{
	int rasterSizeX;
	int rasterSizeY;
	int offsetInBlockX;
	int offsetInBlockY;
	boost::filesystem::path file;

public:
	IndexBlock(int rasterSizeX, int rasterSizeY, int offsetInBlockX, int offsetInBlockY, const boost::filesystem::path& file)
		: rasterSizeX(rasterSizeX)
		, rasterSizeY(rasterSizeY)
		, offsetInBlockX(offsetInBlockX)
		, offsetInBlockY(offsetInBlockY)
		, file(file)
	{}

	int getRasterSizeX() const { return rasterSizeX; }
	int getRasterSizeY() const { return rasterSizeY; }

	int getOffsetInBlockX() const { return offsetInBlockX; }
	int getOffsetInBlockY() const { return offsetInBlockY; }

	const boost::filesystem::path& getFile() const { return file; }
};

class IndexBlocks
{
	std::map<int, std::map<int, IndexBlock>> blocks;
	int blockXSize;
	int blockYSize;
	std::vector<std::int16_t> undefBlockline;
	size_t nrBlocksX;
	size_t nrBlocksY;

public:
	IndexBlocks(const std::vector<IndexLine>& lines);

	boost::optional<const IndexBlock&> getBlock(int blockXOffset, int blockYOffset) const;
	const std::vector<std::int16_t>& getUndefBlockLine() const { return undefBlockline; }

	int getBlockXSize() const;
	int getBlockYSize() const;

	size_t getNrBlocksX() const;
	size_t getNrBlocksY() const;

private:
	void setNrOfBlocks();
};

