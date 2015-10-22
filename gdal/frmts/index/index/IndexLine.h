#pragma once

#include <iosfwd>
#include <string>

#include <boost/filesystem/path.hpp>

#include "IndexWarnings.h"

class IndexLine
{
	boost::filesystem::path path;
	
	int eastMin = 0;
	int eastMax = 0;
	int northMin = 0;
	int northMax = 0;
	int pixelSquareSize = 0;

	bool consistent = true;

public:
	explicit IndexLine(const std::string& line, IndexWarnings& warnings);

	const boost::filesystem::path& getTilePath() const { return path; }

	int getTileEastMin() const { return eastMin; }
	int getTileEastMax() const { return eastMax; }
	int getTileNorthMin() const { return northMin; }
	int getTileNorthMax() const { return northMax; }

	int getPixelSquareSize() const { return pixelSquareSize; }

	bool isConsistent() const { return consistent; }

private:
	void initializeMembers(const std::string& line);
	void checkMembers(IndexWarnings& warnings);
};