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
			dataSet = std::make_unique<IndexDataset>(file, warnings);

		return *dataSet;
	}

	void addTile(int eastMin, int eastMax, int northMin, int northMax, int pixelSquareSize)
	{
		file << "afile " << eastMin << " " << eastMax << " " << northMin << " " << northMax << " " << pixelSquareSize << "\n";
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
	addTile(0, 1, 0, 2, 1);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(2, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeFromMultipleTiles)
{
	addTile(0, 1, 0, 2, 1);
	addTile(-1, 0, 4, 6, 1);
	addTile(3, 4, -8, -6, 1);

	auto& data = getData();

	EXPECT_EQ(5, data.GetRasterXSize());
	EXPECT_EQ(14, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeSkipsInconsistentFirstTile)
{
	addTile(-1, 1, 0, 3, 3);
	addTile(0, 3, 0, 6, 3);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(2, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeIsZeroForOnlyInconsistentTiles)
{
	addTile(-1, 1, 0, 3, 3);
	addTile(0, 1, 0, 2, 3);

	auto& data = getData();

	EXPECT_EQ(0, data.GetRasterXSize());
	EXPECT_EQ(0, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeDependsOnBestResolution)
{
	addTile(0, 1, 0, 1, 1);
	addTile(-1, 3, -1, 3, 4);

	auto& data = getData();

	EXPECT_EQ(1, data.GetRasterXSize());
	EXPECT_EQ(1, data.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeIsInPixelInsteadOfMeters)
{
	addTile(0, 10, 0, 15, 5);

	auto& data = getData();

	EXPECT_EQ(2, data.GetRasterXSize());
	EXPECT_EQ(3, data.GetRasterYSize());
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