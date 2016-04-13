#include "AircomPredRaster/Auxiliary.h"

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
			"tileSizeInPixels": [5, 6],
			"numTiles": [3, 2] })";

		std::wstringstream stream;
		stream << sectionJson;
		boost::property_tree::json_parser::read_json(stream, sectionNode);
	}
};

TEST_F(SectionInfoTests, ParsesJsonNode)
{
	Auxiliary sectionInfo(sectionNode);

	MapBox bounds({1, 3}, {2, 4});
	Auxiliary expected(bounds, GDALDataType::GDT_Float64, {5, 6}, {3, 2});

	EXPECT_EQ(expected, sectionInfo);
}

}}
