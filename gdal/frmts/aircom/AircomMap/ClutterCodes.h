#pragma once

#include <iosfwd>
#include <string>
#include <map>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <cpl_string.h>

namespace aircom { namespace map {

class ClutterCodes
{
	int maxIndex;
	CPLStringList gdalClutterCodes;

public:
	ClutterCodes(const boost::filesystem::path& menuFile);
	ClutterCodes(std::istream& menuFile);

	char** getClutterCodes() { return gdalClutterCodes.List(); }
	size_t getNrOfClutterCodes() { return gdalClutterCodes.size(); }

private:
	void readClutterMenuFile(std::istream& menuFile, std::map<int, std::string>& clutterCodesMap);
	std::pair<int, std::string> readClutterLine(std::string line);
	void createClutterList(std::map<int, std::string>& clutterCodesMap);

};

}}
