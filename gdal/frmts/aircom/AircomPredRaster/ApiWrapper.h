#pragma once

#include "API.h"
#include "ApiParams.h"
#include "GeoParams.h"

#include <boost/optional.hpp>
#include <thread>
#include <unordered_map>
#include <vector>


namespace aircom { namespace pred_raster {

class ApiWrapper
{
	ApiParams params;
	IPredRaster5Ptr predRaster;

	static std::unordered_map<std::thread::id, bool> coInitialized;

	
public:
	ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster = nullptr);

	const ApiParams& getParams() const { return params; }
	IPredRaster5Ptr getPredRaster();

	GeoParams getGeoParams();
	std::vector<unsigned long> getSectionNums();
};

}}
