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
	_REGIONEX r;
	r.m_eastMin = 1;
	r.m_northMax = 2;
	r.m_resolution = 3;
	r.m_width = 4;
	r.m_height = 5;

	EXPECT_CALL(predRaster, raw_GetRegionEx(0, _))
		.WillOnce(DoAll(SetArgPointee<1>(r), Return(S_OK)));

	auto bounds = makeBox(int(r.m_eastMin / 100), int(r.m_northMax - r.m_height * r.m_resolution) / 100,
	                      int(r.m_eastMin + r.m_width * r.m_resolution) / 100, int(r.m_northMax) / 100);
	GeoParams expected(bounds);

	auto actual = wrapper.getGeoParams();
	EXPECT_EQ(expected, actual);
}

TEST_F(ApiWrapperTest, getSectionNums)
{
	std::vector<unsigned long> expected = {7, 11, 3};

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
}

}}