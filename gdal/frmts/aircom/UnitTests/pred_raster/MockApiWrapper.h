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
	MOCK_METHOD1(getSectionInfo, SectionInfo(unsigned long sectionNum));
	MOCK_METHOD0(getSectionInfos, std::map<unsigned long, SectionInfo>());
};

}}
