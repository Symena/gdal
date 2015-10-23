#pragma once

#include <boost/filesystem/path.hpp>

#include "gdal_pam.h"

#include "IndexDataset.h"
#include "IndexBlocks.h"

class IndexRasterBand: public GDALPamRasterBand
{
	IndexBlocks blocks;

public:
	IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks);
	
	virtual CPLErr IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage) override;
};
