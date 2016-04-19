#pragma once

#include "API.h"
#include "ApiParams.h"
#include "Auxiliary.h"

#include <vector>

namespace aircom { namespace pred_raster {

class ApiWrapper
{
	ApiParams params;
	IPredRaster5Ptr predRaster;

public:
	ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster = nullptr);

	const ApiParams& getParams() const { return params; }
	IPredRaster5Ptr getPredRaster();

	virtual Auxiliary getAuxiliary();

private:
	std::vector<unsigned long> getSectionNums();
	GDALDataType getDataType(unsigned long sectionNum);
	Point getTileSizeInPixels(unsigned long sectionNum);
	Rectangle getBounds(unsigned long sectionNum, int& epsg);
	SectionInfos getSectionInfos();
};

}}
