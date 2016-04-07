#pragma once

#include "API.h"
#include "ApiParams.h"
#include "GeoParams.h"

namespace aircom { namespace pred_raster {

class ApiWrapper
{
	ApiParams params;
	IPredRaster5Ptr predRaster;

public:
	explicit ApiWrapper(ApiParams apiParams) : params(std::move(apiParams)) {}

	const ApiParams& getParams() const { return params; }
	IPredRaster5Ptr getPredRaster();
	GeoParams getGeoParams();
};

}}
