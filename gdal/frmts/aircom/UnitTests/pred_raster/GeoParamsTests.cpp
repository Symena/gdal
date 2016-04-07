#include "AircomPredRaster/GeoParams.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;

struct GeoParamsTests : public testing::Test
{
	wptree geoNode;

	GeoParamsTests()
	{
		geoNode.add<int>(L"left", 1);
		geoNode.add<int>(L"right", 2);
		geoNode.add<int>(L"bottom", 3);
		geoNode.add<int>(L"top", 4);

	}
};

TEST_F(GeoParamsTests, ParsesBounds)
{
	GeoParams geoParams(geoNode);

	MapPoint bottomLeft(1, 3);
	MapPoint topRight(2, 4);
	MapBox expected(bottomLeft, topRight);

	EXPECT_EQ(expected, geoParams.boundingBox);
}

TEST_F(GeoParamsTests, ParsesProjection)
{
	// TODO
}

}}