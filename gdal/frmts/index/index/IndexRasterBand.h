#pragma once

#include <boost/filesystem/path.hpp>

#include "gdal_pam.h"
#include "IndexDataset.h"

class IndexLine;

class IndexRasterBand: public GDALPamRasterBand
{
	boost::filesystem::path referencedFile;

public:
	IndexRasterBand(IndexDataset* owningDataSet, const IndexLine& line);
	
	virtual CPLErr IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage) override;
};
