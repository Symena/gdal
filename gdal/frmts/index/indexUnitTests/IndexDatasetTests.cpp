#include <gmock/gmock.h>

#include <boost/filesystem/path.hpp>

#include "IndexDataset.h"

class IndexDatasetTests : public testing::Test
{
	std::stringstream file;
	IndexWarnings warnings;

	std::unique_ptr<IndexDataset> dataSet;

public:

	IndexDataset& getData()
	{
		if(!dataSet)
			dataSet = std::make_unique<IndexDataset>(file, std::unique_ptr<std::istream>{}, warnings);

		return *dataSet;
	}

	void addBlock(int eastMin, int eastMax, int northMin, int northMax, int resolution)
	{
		file << "afile " << eastMin << " " << eastMax << " " << northMin << " " << northMax << " " << resolution << "\n";
	}
};

TEST_F(IndexDatasetTests, sizeOfInitialRasterIsZero)
{
	auto& data = getData();

	EXPECT_EQ(0, data.GetRasterXSize());
	EXPECT_EQ(0, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeFromSingleTile)
{
	addBlock(0, 1, 0, 2, 1);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(2, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeFromMultipleFullTiles)
{
	addBlock(0, 1, 0, 2, 1);
	addBlock(-1, 0, 4, 6, 1);
	addBlock(3, 4, -8, -6, 1);

	auto& data = getData();

	EXPECT_EQ(5, data.GetRasterXSize());
	EXPECT_EQ(14, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeFromPartialTiles)
{
	addBlock(0, 2, 0, 2, 1);
	addBlock(0, 2, 2, 3, 1);
	addBlock(-1, 0, 0, 2, 1);

	auto& data = getData();

	EXPECT_EQ(4, data.GetRasterXSize());
	EXPECT_EQ(4, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeSkipsInconsistentFirstTile)
{
	addBlock(-1, 1, 0, 3, 3);
	addBlock(0, 3, 0, 6, 3);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(2, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeIsZeroForOnlyInconsistentTiles)
{
	addBlock(-1, 1, 0, 3, 3);
	addBlock(0, 1, 0, 2, 3);

	auto& data = getData();

	EXPECT_EQ(0, data.GetRasterXSize());
	EXPECT_EQ(0, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeDependsOnBestResolution)
{
	addBlock(0, 1, 0, 1, 1);
	addBlock(-1, 3, -1, 3, 4);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(1, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeIsInPixelInsteadOfMeters)
{
	addBlock(0, 10, 0, 15, 5);

	auto& data = getData();

	EXPECT_EQ(2, data.GetRasterXSize());
	EXPECT_EQ(3, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, providesResolutionsAsMetadata)
{
	addBlock(0, 2, 0, 2, 1);
	addBlock(0, 2, 2, 3, 1);
	addBlock(-1, 0, 0, 2, 2);

	auto& data = getData();

	auto** resolutions = data.GetMetadata("Resolutions");	

	EXPECT_EQ("1m=2 tiles", std::string(resolutions[0]));
	EXPECT_FALSE(resolutions[1]);
	// When multi res is supported
	// EXPECT_EQ("2m=1 tiles", std::string(resolutions[0]));
}

TEST_F(IndexDatasetTests, setsGeoTransformAccordingToBoundingBox)
{
	addBlock(1, 3, 3, 4, 1);
	addBlock(3, 4, 4, 6, 1);

	auto& data = getData();

	double actual[6];	
	data.GetGeoTransform(actual);

	double expected[6];	
	expected[0] = 1; // east min
	expected[1] = 1; // resolution is always 1;
	expected[2] = 0;
    expected[3] = 6; // north max
	expected[4] = 0;
	expected[5] = -1; // resolution is always 1;
		
	ASSERT_THAT(actual, ::testing::ElementsAreArray(expected));
}

//support for different resolutions

class IndexDatasetIdentifyTests : public ::testing::Test
{
	boost::filesystem::path fileName = "index.txt";
	std::stringstream header;

public:
	void setFileName(const std::string& fileName) { this->fileName = fileName; }
	void setHeader(const std::string& header) { this->header << header; }

	bool identify()
	{
		return IndexDataset::Identify(fileName, header);
	}
};

TEST_F(IndexDatasetIdentifyTests, IdentificationFailsOnWrongFileName)
{
	setFileName("afile.txt");

	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetIdentifyTests, IdentificationFailsOnEmptyHeader)
{
	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetIdentifyTests, IdentificationFailsOnMalformedHeader)
{
	setHeader("asdklöfjöasldkfj");

	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetIdentifyTests, IdentificationWorksIfFirstLineCanBeParsed)
{
	setHeader("file 0 1 0 1 1 \n garbage");

	EXPECT_TRUE(identify());
}