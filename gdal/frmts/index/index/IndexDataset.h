#pragma once

#include "gdal_pam.h"

#include <iosfwd>

#include <boost/filesystem/path.hpp>

#include "IndexWarnings.h"

class IndexDataset: public GDALPamDataset
{
public:
	IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings);
	IndexDataset(std::istream& indexFile, IndexWarnings& warnings);
	
	static GDALDataset* Open(GDALOpenInfo* openInfo);

	static bool Identify(const boost::filesystem::path& file, std::istream& header);
};
