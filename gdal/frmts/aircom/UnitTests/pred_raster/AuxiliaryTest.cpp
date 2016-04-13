#include "AircomPredRaster/Auxiliary.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;

struct AuxiliaryTest : public testing::Test
{
	wptree auxiliaryNode;

	AuxiliaryTest()
	{
		std::wstring sectionJson = LR"({
			"BoundingBox": { "BottomLeft": [1, 3], "TopRight": [2, 4] },
			"SectionDataTypes": { "0": "R64" },
			"TileSizeInPixels": [5, 6] })";

		std::wstringstream stream;
		stream << sectionJson;
		boost::property_tree::json_parser::read_json(stream, auxiliaryNode);
	}
};

TEST_F(AuxiliaryTest, ParsesJsonNode)
{
	Auxiliary sectionInfo(auxiliaryNode);

	MapBox bounds({1, 3}, {2, 4});
	Auxiliary expected(bounds, { {0, GDT_Float64} }, {5, 6});

	EXPECT_EQ(expected, sectionInfo);
}

}}
