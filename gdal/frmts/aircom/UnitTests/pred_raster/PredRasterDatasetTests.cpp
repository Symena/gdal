#include "AircomPredRaster/Dataset.h"

#include "MockApiWrapper.h"

#include <gmock/gmock.h>

#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;
using namespace testing;

struct PredRasterDatasetTests : public Test
{
	wptree gapTree;
	Warnings warnings;

	wptree sampleGapTree;
	wptree& apiNode;

	std::shared_ptr<MockApiWrapper> apiWrapper;

	PredRasterDatasetTests()
		: sampleGapTree(createSampleGapTree())
		, apiNode(sampleGapTree.get_child(L"EnterprisePredRasterApi"))
		, apiWrapper(std::make_shared<MockApiWrapper>(ApiParams(apiNode)))
	{}

	static wptree createSampleGapTree()
	{
		std::wstring json = LR"({
			"EnterprisePredRasterApi": {
				"PredictionFolder":  "z:/ombie",
				"PredAccessClassID": "{12345678-FAKE-407A-AA74-AEF504C165EE}",
				"PredRasterClassID": "{12345678-FAKE-407A-AA74-AEF504C165EE}",
				"PredData": {
					"nX_cm": 100,
					"nY_cm": 200,
					"nAntennaHeight_cm": 300,
					"nGroundHeight_cm": 400,
					"nResolution_cm": 500,
					"nRadius_cm": 600,
					"fFrequency_MHz": 7.1,
					"nModelCRC": 8,
					"nPredFlags": 9,
					"nAntennaCRC": 10,
					"fAntennaMechanicalTilt_deg": 11.1,
					"fAntennaAzimuth_deg": 12.1,
					"nCwWeight": 13,
					"fCwRolloff": 14.1
				},
				"Section": "pathloss"
			},
			"Auxiliary": {
				"BoundingBox": {
					"BottomLeft": [1, 3],
					"TopRight": [6, 13]
				},

				"TileSizeInPixels": [3, 4],

				"SectionDataTypes": {
					"0": "R64"
				}
			}
		})";

		std::wstringstream stream;
		stream << json;
		
		wptree ret;
		boost::property_tree::json_parser::read_json(stream, ret);
		return ret;
	}
};

TEST_F(PredRasterDatasetTests, ExceptionOnIncompleteData)
{
	EXPECT_THROW(Dataset(wptree(), warnings), boost::property_tree::ptree_bad_path);
}

TEST_F(PredRasterDatasetTests, ParsesGapSuccessful)
{
	Dataset dataset(sampleGapTree, warnings);
	EXPECT_EQ(0, warnings.size());
}

TEST_F(PredRasterDatasetTests, CollectsMetaData)
{
	wptree metaDomain;
	metaDomain.add(L"foo", 1);
	metaDomain.add(L"bar", 2);

	wptree meta;
	meta.add_child(L"dom", metaDomain);

	sampleGapTree.add_child(L"Meta", meta);

	Dataset dataset(sampleGapTree, warnings);

	EXPECT_EQ(0, warnings.size());

	auto domainList = dataset.GetMetadataDomainList();
	ASSERT_NE(nullptr, *domainList);

	EXPECT_EQ(std::string("dom"), *domainList);
	EXPECT_EQ(std::string("1"), dataset.GetMetadataItem("foo", "dom"));
	EXPECT_EQ(std::string("2"), dataset.GetMetadataItem("bar", "dom"));
}

TEST_F(PredRasterDatasetTests, LoadsBoundingBoxFromGapFile)
{
	Dataset dataset(sampleGapTree, warnings);

	MapBox bounds({1, 3}, {6, 13});

	EXPECT_EQ(bounds, dataset.getBoundingBox()); 
	EXPECT_EQ(1, dataset.GetRasterXSize());
	EXPECT_EQ(2, dataset.GetRasterYSize());

	double transformMatrix[6];
	dataset.GetGeoTransform(transformMatrix);
	EXPECT_THAT(transformMatrix, testing::ElementsAre( 1, 5,  0,
	                                                  13, 0, -5));
}

TEST_F(PredRasterDatasetTests, ExceptionOnInvalidDimensions)
{
	wptree bottomLeft;
	std::wstringstream json;
	json << L"[10, 3]";
	boost::property_tree::json_parser::read_json(json, bottomLeft);

	sampleGapTree.get_child(L"Sections.0.bottomLeft").swap(bottomLeft);

	EXPECT_THROW(Dataset(sampleGapTree, warnings), std::runtime_error);
}

TEST_F(PredRasterDatasetTests, OnlyOneBandWhenSectionSpecified)
{
	Dataset dataset(sampleGapTree, warnings);
	EXPECT_EQ(1, dataset.GetRasterCount());
}

TEST_F(PredRasterDatasetTests, LoadSectionsFromApi)
{
	sampleGapTree.erase(L"Sections");

	std::map<unsigned long, Auxiliary> sectionInfos;
	sectionInfos.emplace(0, Auxiliary({{0, 0}, {10, 10}}, GDALDataType::GDT_Byte, {1, 1}, {1, 1}));

	EXPECT_CALL(*apiWrapper, getSectionInfos()).WillOnce(Return(sectionInfos));

	Dataset(sampleGapTree, apiWrapper, warnings);
}

}}
