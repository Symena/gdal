#include "AircomPredRaster/ApiWrapper.h"

#include "MockPredRaster.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <sstream>

using namespace ::testing;

namespace aircom { namespace pred_raster {

struct ApiWrapperTest : public Test
{
	ApiParams params;
	MockPredRaster predRaster;
	ApiWrapper wrapper;

	ApiWrapperTest()
		: params("z:/ombie", PredData{}, L"123-456", L"567-890", Section::Unspecified)
		, predRaster()
		, wrapper(params, &predRaster)
	{}

};

TEST_F(ApiWrapperTest, getGeoParams)
{
	_REGIONEX region;
	region.m_eastMin = 1;
	region.m_northMax = 2;
	region.m_resolution = 3;
	region.m_width = 4;
	region.m_height = 5;

	EXPECT_CALL(predRaster, raw_GetRegionEx(0, _))
		.WillOnce(DoAll(SetArgPointee<1>(region), Return(S_OK)));

	MapBox bounds = makeBox(region.m_eastMin, region.m_northMax - region.m_height, 
							region.m_eastMin + region.m_width, region.m_northMax);
	GeoParams expected(bounds);

	auto actual = wrapper.getGeoParams();
	EXPECT_EQ(expected, actual);
}

TEST_F(ApiWrapperTest, getSectionNums)
{
	std::vector<unsigned long> expected = {7, 11, 4};

	EXPECT_CALL(predRaster, raw_GetNumSections(_))
		.WillOnce(DoAll(SetArgPointee<0>(3), Return(S_OK)));

	EXPECT_CALL(predRaster, raw_GetSectionNumList(3, _))
		.WillOnce(Invoke([&](auto numSections, auto* sectionNums) { 
			sectionNums[0] = 7;
			sectionNums[1] = 11;
			sectionNums[2] = 3;
			return S_OK; }));
		
	auto actual = wrapper.getSectionNums();
	EXPECT_EQ(expected, actual);

	wrapper.getSectionNums();
}

}}