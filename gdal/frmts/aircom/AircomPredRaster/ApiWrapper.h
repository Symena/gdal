#pragma once

#include "API.h"
#include "ApiParams.h"
#include "GeoParams.h"

#include <boost/optional.hpp>
#include <vector>

namespace aircom { namespace pred_raster {

class ApiWrapper
{
	ApiParams params;
	IPredRaster5Ptr predRaster;
	
	boost::optional<std::vector<unsigned long>> sections;

public:
	ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster= nullptr)
		: params(std::move(apiParams)) 
		, predRaster(predRaster)
	{}	

	const ApiParams& getParams() const { return params; }
	IPredRaster5Ptr getPredRaster();


	GeoParams getGeoParams();
	const std::vector<unsigned long>& getSectionNums();
};

}}
