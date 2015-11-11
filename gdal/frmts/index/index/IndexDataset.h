#pragma once

#include "gdal_pam.h"

#include <iosfwd>
#include <vector>
#include <memory>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include "IndexWarnings.h"
#include "IndexBlocks.h"
#include "IndexClutterCodes.h"

class IndexLine;

class IndexDataset: public GDALPamDataset
{
public:
	IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings);
	IndexDataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, IndexWarnings& warnings);
	
	static GDALDataset* Open(GDALOpenInfo* openInfo);
	static bool Identify(const boost::filesystem::path& file, std::istream& header);

	IndexBlocks& getBlocks() { return blocks; }
	boost::optional<IndexClutterCodes>& getClutterCodes() { return clutterCodes; }

private:
	IndexBlocks blocks;
	boost::optional<IndexClutterCodes> clutterCodes;

	void setBoundingBox();

	std::vector<IndexLine> readLines(std::istream& indexFile, IndexWarnings& warnings);
	boost::optional<IndexClutterCodes> readClutterCodes(std::unique_ptr<std::istream> clutterFile);
	void provideResolutionsAsMetadata(const std::vector<IndexLine>& lines);
};
