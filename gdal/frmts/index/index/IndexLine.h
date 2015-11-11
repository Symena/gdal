#pragma once

#include <iosfwd>
#include <string>
#include <memory>

#include "IndexWarnings.h"
#include "IndexStreamSource.h"

class IndexLine
{	
	int eastMin = 0;
	int eastMax = 0;
	int northMin = 0;
	int northMax = 0;
	int resolution = 0;

	std::shared_ptr<IndexStreamSource> dataSource;

	bool consistent = true;

public:
	IndexLine(const std::string& line, IndexWarnings& warnings);
	IndexLine(int eastMin, int eastMax, int northMin, int northMax, int resolution, std::shared_ptr<IndexStreamSource> dataSource)
		: eastMin(eastMin)
		, eastMax(eastMax)
		, northMin(northMin)
		, northMax(northMax)
		, resolution(resolution)
		, dataSource(std::move(dataSource))
	{}

	const std::shared_ptr<IndexStreamSource>& getTileDataSource() const { return dataSource; }

	int getTileEastMin() const { return eastMin; }
	int getTileEastMax() const { return eastMax; }
	int getTileNorthMin() const { return northMin; }
	int getTileNorthMax() const { return northMax; }

	int getResolution() const { return resolution; }

	bool isConsistent() const { return consistent; }

private:
	void initializeMembers(const std::string& line);
	void checkMembers(IndexWarnings& warnings);
};