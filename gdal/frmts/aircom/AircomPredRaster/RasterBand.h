#pragma once

#include "gdal_pam.h"

#include "Dataset.h"

namespace aircom { namespace pred_raster {

class RasterBand: public GDALPamRasterBand
{
	int section;
public:
	RasterBand(Dataset* owningDataSet, int bandIndex, int section);
	
	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

private:
	Dataset& getDataset() { return *static_cast<Dataset*>(poDS); }
};

}}
