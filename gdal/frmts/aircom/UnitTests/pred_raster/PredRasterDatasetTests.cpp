#include "AircomPredRaster/Dataset.h"

#include <gmock/gmock.h>

#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace aircom { namespace pred_raster {

struct PredRasterDatasetTests : public testing::Test
{
	std::wstringstream gapFile;
	boost::property_tree::wptree gapTree;
	Warnings warnings;

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

TEST_F(PredRasterDatasetTests, WarningsOnMissingAPIParams)
{
	gapTree.add(L"API", L"");	
	
	EXPECT_THROW(Dataset(gapFile, warnings), std::runtime_error);
	ASSERT_EQ(3, warnings.size());
}

}}
