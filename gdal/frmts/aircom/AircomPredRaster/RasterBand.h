#pragma once

#include "gdal_pam.h"

#include "Dataset.h"

namespace aircom { namespace pred_raster {

class RasterBand : public GDALPamRasterBand
{
	std::shared_ptr<ApiWrapper> apiWrapper;
	const unsigned long sectionNum;

public:
	RasterBand(Dataset* owningDataSet, int bandIndex, std::shared_ptr<ApiWrapper> apiWrapper,
		unsigned long sectionNum, const Auxiliary& sectionInfo);

	virtual GDALColorInterp GetColorInterpretation() override;

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

private:
	IPredRaster5Ptr getPredRaster() { return apiWrapper->getPredRaster(); }
};

}}
