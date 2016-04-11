#pragma once

#include "API.h"
#include "ApiParams.h"
#include "SectionInfo.h"

#include <boost/optional.hpp>
#include <thread>
#include <unordered_map>
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

	std::vector<unsigned long> getSectionNums();
	SectionInfo getSectionInfo(unsigned long sectionNum);
};

}}
