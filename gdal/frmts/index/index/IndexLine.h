#pragma once

#include <iosfwd>

#include <boost/filesystem/path.hpp>

class IndexLine
{
	boost::filesystem::path path;
	
	int eastMin = 0;
	int eastMax = 0;
	int northMin = 0;
	int northMax = 0;
	int pixelSquareSize = 0;

public:
	IndexLine(std::istream& indexFile);

	const boost::filesystem::path& getTilePath() const { return path; }

	int getTileEastMin() const { return eastMin; }
	int getTileEastMax() const { return eastMax; }
	int getTileNorthMin() const { return northMin; }
	int getTileNorthMax() const { return northMax; }

	int getPixelSquareSize() const { return pixelSquareSize; }

private:
	void checkStreamValidity(const std::istream& indexFile);
	void initializeMembers(std::istream& indexFile);
	void checkMembers();
};