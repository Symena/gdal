#include "AircomPredRaster/ApiWrapper.h"

#include "MockPredRaster.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

struct ApiWrapperTest : public testing::Test
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

	EXPECT_CALL(predRaster, raw_GetRegionEx(0, &region))
		.Times(1);

	MapPoint topLeft(region.m_eastMin, region.m_northMax);
	MapPoint bottomRight(region.m_eastMin + region.m_width, region.m_northMax - region.m_height);
	MapBox bounds(topLeft, bottomRight);
	GeoParams expected(bounds);

	auto actual = wrapper.getGeoParams();
	EXPECT_EQ(expected, actual);
}

}}