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

	PredRasterDatasetTests()
	{
		wptree apiNode;
		apiNode.add(L"PredictionFolder", L"c:/pred");
		apiNode.add(L"PredAccessClassID", L"{98A67A67-9DAB-407A-AA74-AEF504C165EE}");
		apiNode.add(L"PredRasterClassID", L"{12345678-9DAB-407A-AA74-AEF504C165EE}");

		wptree predDataNode;
		predDataNode.add<std::int64_t>(L"nX_cm", 1);
		predDataNode.add<std::int64_t>(L"nY_cm", 2);

		predDataNode.add<std::int32_t>(L"nAntennaHeight_cm", 3);
		predDataNode.add<std::int32_t>(L"nGroundHeight_cm", 4);
		predDataNode.add<std::uint32_t>(L"nResolution_cm", 5);
		predDataNode.add<std::uint32_t>(L"nRadius_cm", 6);

		predDataNode.add<float>(L"fFrequency_MHz", 7.1f);
		predDataNode.add<std::uint64_t>(L"nModelCRC", 8);
		predDataNode.add<std::uint32_t>(L"nPredFlags", 9);
		predDataNode.add<std::uint64_t>(L"nAntennaCRC", 10);

		predDataNode.add<double>(L"fAntennaMechanicalTilt_deg", 11.1);
		predDataNode.add<double>(L"fAntennaAzimuth_deg", 12.1);
		predDataNode.add<std::uint16_t>(L"nCwWeight", 13);
		predDataNode.add<float>(L"fCwRolloff", 14.1f);

		apiNode.add_child(L"PredData", predDataNode);

		sampleGapTree.add_child(L"EnterprisePredRasterApi", apiNode);

		wptree geoNode;
		geoNode.add(L"left", 1);
		geoNode.add(L"right", 2);
		geoNode.add(L"bottom", 3);
		geoNode.add(L"top", 5);	

		sampleGapTree.add_child(L"Geo", geoNode);
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
	
	MapBox bounds({1, 3}, {2, 5});

	EXPECT_EQ(bounds, dataset.getBoundingBox()); 
	EXPECT_EQ(1, dataset.GetRasterXSize());
	EXPECT_EQ(2, dataset.GetRasterYSize());
}

TEST_F(PredRasterDatasetTests, ExceptionOnInvalidDimensions)
{	
	sampleGapTree.get_child(L"Geo").put(L"left", 10);
	
	EXPECT_THROW(Dataset dataset(sampleGapTree, warnings), std::runtime_error);
}

}}
