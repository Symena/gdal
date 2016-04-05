#pragma once

#include "gdal_pam.h"

#include "Dataset.h"

namespace aircom_pred_raster {

class RasterBand: public GDALPamRasterBand
{
public:
	RasterBand(Dataset* owningDataSet, int bandIndex = 1);
	
	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

private:
	Dataset& getDataset() { return *static_cast<Dataset*>(poDS); }
};

}
