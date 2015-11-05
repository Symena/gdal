#pragma once

#include "gdal_pam.h"

#include <iosfwd>
#include <memory>

#include <boost/filesystem/path.hpp>

class IndexClutterCodes
{
	int maxIndex;
	std::unique_ptr<std::vector<std::string>> clutterCodes;

public:
	IndexClutterCodes(const boost::filesystem::path& menuFile);
	IndexClutterCodes(std::istream& menuFile);

	const std::vector<std::string>& getClutterCodes() const { return *clutterCodes; }

private:
	void readClutterMenuFile(std::istream& menuFile, std::map<int, std::string>& clutterCodesMap);
	std::pair<int, std::string> readClutterLine(std::string line);
	void createClutterList(std::map<int, std::string>& clutterCodesMap);

};
