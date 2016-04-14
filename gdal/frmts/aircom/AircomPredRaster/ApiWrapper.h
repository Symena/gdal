#pragma once

#include "API.h"
#include "ApiParams.h"
#include "Auxiliary.h"

#include <boost/optional.hpp>
#include <map>
#include <thread>
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

	virtual std::vector<unsigned long> getSectionNums();
	virtual GDALDataType getDataType(unsigned long sectionNum);
	virtual std::map<unsigned long, GDALDataType> getSectionDataTypes();
	virtual Auxiliary getAuxiliary();
};

}}
