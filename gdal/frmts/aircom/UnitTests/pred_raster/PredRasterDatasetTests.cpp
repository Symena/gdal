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

}}
