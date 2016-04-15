#pragma once

#include "API.h"
#include "ApiParams.h"
#include "Auxiliary.h"

#include <map>
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
	MapPoint getTileSizeInPixels(unsigned long sectionNum);
	SectionInfos getSectionInfos();
};

}}
