#include "AircomPredRaster/SectionInfo.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;

struct SectionInfoTests : public testing::Test
{
	wptree sectionNode;

	SectionInfoTests()
	{
		std::wstring sectionJson = LR"({
			"bottomLeft": [1, 3],
			"topRight": [2, 4],
			"dataType": "R64",
			"tileSizeInPixels": [5, 6] })";

		std::wstringstream stream;
		stream << sectionJson;
		boost::property_tree::json_parser::read_json(stream, sectionNode);
	}
};

TEST_F(SectionInfoTests, ParsesBounds)
{
	SectionInfo sectionInfo(sectionNode);

	MapPoint bottomLeft(1, 3);
	MapPoint topRight(2, 4);
	MapBox bounds(bottomLeft, topRight);

	SectionInfo expected(bounds, GDALDataType::GDT_Float64, {5, 6});

	EXPECT_EQ(expected, sectionInfo);
}

}}