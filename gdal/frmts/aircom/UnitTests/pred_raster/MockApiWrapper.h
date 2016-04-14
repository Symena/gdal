#pragma once

#include "AircomPredRaster/ApiWrapper.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockApiWrapper : public ApiWrapper
{

	MockApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster = nullptr)
		: ApiWrapper(std::move(apiParams), predRaster)
	{}

	MOCK_METHOD0(getSectionNums, std::vector<unsigned long>());	
	MOCK_METHOD1(getDataType, GDALDataType(unsigned long sectionNum));
	MOCK_METHOD0(getSectionDataTypes, std::map<unsigned long, GDALDataType>());
	MOCK_METHOD0(getAuxiliary, Auxiliary());
};

}}
