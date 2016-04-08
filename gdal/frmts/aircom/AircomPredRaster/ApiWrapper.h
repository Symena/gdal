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
	ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster= nullptr)
		: params(std::move(apiParams)) 
		, predRaster(predRaster)
	{}	

	const ApiParams& getParams() const { return params; }
	IPredRaster5Ptr getPredRaster();
	GeoParams getGeoParams();
};

}}
