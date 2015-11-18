#pragma once

#include "gdal_pam.h"

#include "IndexDataset.h"

class IndexRasterBand: public GDALPamRasterBand
{
public:
	IndexRasterBand(IndexDataset* owningDataSet, int bandIndex = 1);
	
	virtual char** GetCategoryNames() override;

	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

	virtual CPLErr IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
		void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
		GSpacing nPixelSpace, GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg) override;

private:
	IndexDataset& getDataset() { return *static_cast<IndexDataset*>(poDS); }
};
