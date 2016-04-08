#pragma once

#include "gdal_pam.h"

#include "Dataset.h"

namespace aircom { namespace pred_raster {

class RasterBand: public GDALPamRasterBand
{
	const int section;

public:
	RasterBand(Dataset* owningDataSet, int bandIndex, int section);

	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
		void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
		GSpacing nPixelSpace, GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg) override;

	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

private:
	Dataset& getDataset() { return *static_cast<Dataset*>(poDS); }
	IPredRaster5Ptr getPredRaster() { return getDataset().getPredRaster(); }

	void initializeBlocksInfo();
};

}}
