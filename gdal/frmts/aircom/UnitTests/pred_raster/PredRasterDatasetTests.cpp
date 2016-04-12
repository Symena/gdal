#include "AircomPredRaster/Dataset.h"

#include <gmock/gmock.h>

#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;

struct PredRasterDatasetTests : public testing::Test
{
	std::wstringstream gapFile;
	wptree gapTree;
	Warnings warnings;

	wptree sampleGapTree;
	wptree& apiNode;
	

	PredRasterDatasetTests()
		: sampleGapTree(createSampleGapTree())
		, apiNode(sampleGapTree.get_child(L"EnterprisePredRasterApi"))
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
			"Sections": {
				"0": {
					"bottomLeft": [1, 3],
					"topRight": [6, 13],
					"dataType": "R64",
					"tileSizeInPixels": [3, 4]
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

TEST_F(PredRasterDatasetTests, ExceptionOnEmptyFile)
{
	EXPECT_THROW(Dataset(gapFile, warnings), boost::property_tree::json_parser_error);
}

TEST_F(PredRasterDatasetTests, ExceptionOnInvalidJson)
{
	gapFile << L"invalid";
	EXPECT_THROW(Dataset(gapFile, warnings), boost::property_tree::json_parser_error);
}

TEST_F(PredRasterDatasetTests, ExceptionOnIncompleteData)
{
	gapFile << L"{}";
	EXPECT_THROW(Dataset(gapFile, warnings), boost::property_tree::ptree_bad_path);
	EXPECT_THROW(Dataset(gapTree, warnings), boost::property_tree::ptree_bad_path);
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

TEST_F(PredRasterDatasetTests, LoadsGeoParamsFromGapFile)
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
	apiNode.add(L"Section", L"Inclination");
	sampleGapTree.put_child(L"EnterprisePredRasterApi", apiNode);

	Dataset dataset(sampleGapTree, warnings);

	EXPECT_EQ(1, dataset.GetRasterCount());
}

}}
