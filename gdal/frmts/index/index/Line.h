#pragma once

#include <boost/filesystem/path.hpp>
#include <iosfwd>
#include <string>
#include <memory>

#include "StreamSource.h"
#include "Warnings.h"

namespace aircom_map {

class Line
{
	int eastMin = 0;
	int eastMax = 0;
	int northMin = 0;
	int northMax = 0;
	int resolution = 0;

	std::shared_ptr<StreamSource> dataSource;

	bool consistent = true;

public:
	Line(const std::string& line, Warnings& warnings, const boost::filesystem::path& dataRoot);
	Line(int eastMin, int eastMax, int northMin, int northMax, int resolution, std::shared_ptr<StreamSource> dataSource)
		: eastMin(eastMin)
		, eastMax(eastMax)
		, northMin(northMin)
		, northMax(northMax)
		, resolution(resolution)
		, dataSource(std::move(dataSource))
	{}

	const std::shared_ptr<StreamSource>& getTileDataSource() const { return dataSource; }

	int getTileEastMin() const { return eastMin; }
	int getTileEastMax() const { return eastMax; }
	int getTileNorthMin() const { return northMin; }
	int getTileNorthMax() const { return northMax; }

	int getResolution() const { return resolution; }

	bool isConsistent() const { return consistent; }

private:
	void initializeMembers(const std::string& line, const boost::filesystem::path& dataRoot);
	void checkMembers(Warnings& warnings);
};

}
