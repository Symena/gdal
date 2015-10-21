#pragma once

#include "gdal_pam.h"

#include <iosfwd>

#include <boost/filesystem/path.hpp>

class IndexDataset: public GDALPamDataset
{
public:
	~IndexDataset();

	static GDALDataset* Open(GDALOpenInfo* openInfo);

	static bool Identify(const boost::filesystem::path& file, std::istream& header);
};
