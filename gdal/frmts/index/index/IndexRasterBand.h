#pragma once

#include <boost/filesystem/path.hpp>

#include "gdal_pam.h"

#include "IndexDataset.h"
#include "IndexBlocks.h"

class IndexRasterBand: public GDALPamRasterBand
{
	IndexBlocks blocks;

	std::vector<std::int16_t> undefValueLineBigEndian;
	std::vector<std::int16_t> undefValueLineLittleEndian;

public:
	IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks);
	
	virtual CPLErr IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage) override;

	void convertToNativeByteOrder(void* pImage);

	virtual GDALColorInterp GetColorInterpretation() override;

private:
	void fillBlockWithUndefLittleEndian(std::ostream& outputStream);
	void fillBlockWithUndefBigEndian(std::ostream& outputStream);
	void fillBlock(std::ostream& outputStream, const std::vector<std::int16_t>& lineData);

	bool tileCoversBlockCompletely(const MapBox& tileBox, const MapBox& blockBox);
};
