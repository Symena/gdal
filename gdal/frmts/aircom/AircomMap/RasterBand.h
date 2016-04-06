#pragma once

#include "gdal_pam.h"

#include "Dataset.h"

namespace aircom_map {

class RasterBand: public GDALPamRasterBand
{
public:
	RasterBand(Dataset* owningDataSet, int bandIndex = 1);
	
	virtual char** GetCategoryNames() override;

	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

	virtual CPLErr IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
		void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
		GSpacing nPixelSpace, GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg) override;

private:
	Dataset& getDataset() { return *static_cast<Dataset*>(poDS); }
};

}
