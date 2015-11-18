#include "IndexDataset.h"

#include "IndexBlocksBuilder.h"

#include <gmock/gmock.h>

class IndexDatasetTests : public testing::Test
{
	std::unique_ptr<std::stringstream> clutterFile;
	std::unique_ptr<IndexDataset> dataset;

protected:
	IndexBlocksBuilder builder;

	IndexDataset& getDataset()
	{
		if (!dataset)
			dataset = std::make_unique<IndexDataset>(builder.create(), std::move(clutterFile));

		return *dataset;
	}

	void addBlock(int minX, int minY, int maxX, int maxY, int resolution)
	{
		builder.addBlock().from(minX, minY).to(maxX, maxY).resolution(resolution);
	}

	void addClutter(int code, const std::string& name)
	{
		if (!clutterFile)
			clutterFile = std::make_unique<std::stringstream>();

		*clutterFile << std::to_string(code) << " " << name.c_str() << "\n";
	}

	std::vector<std::int16_t> readPixels(const MapBox& sourceRegion, int widthInPixels, int heightInPixels,
		GDALRIOResampleAlg algorithm)
	{
		std::vector<std::int16_t> pixels(static_cast<size_t>(widthInPixels) * heightInPixels);

		GDALRasterIOExtraArg extraArg = { 0 };
		extraArg.nVersion = RASTERIO_EXTRA_ARG_CURRENT_VERSION;
		extraArg.eResampleAlg = algorithm;

		auto error = getDataset().RasterIO(GDALRWFlag::GF_Read,
			sourceRegion.min_corner().get<0>(), sourceRegion.min_corner().get<1>(),
			width(sourceRegion), height(sourceRegion),
			pixels.data(), widthInPixels, heightInPixels, GDALDataType::GDT_Int16,
			1, nullptr, // 1 band is required
			0, 0, 0,
			&extraArg);
		EXPECT_EQ(CPLErr::CE_None, error);

		return pixels;
	}
};

TEST_F(IndexDatasetTests, sizeOfInitialRasterIsZero)
{
	auto& ds = getDataset();

	EXPECT_EQ(0, ds.GetRasterXSize());
	EXPECT_EQ(0, ds.GetRasterYSize());
}

TEST_F(IndexDatasetTests, rasterSizeFromMultipleBlocks)
{
	addBlock( 0, 0, 1, 2, 1);
	addBlock(-1, 4, 0, 6, 1);
	addBlock( 3,-8, 4,-6, 1);

	auto& ds = getDataset();

	EXPECT_EQ(5, ds.GetRasterXSize());
	EXPECT_EQ(14, ds.GetRasterYSize());
}

TEST_F(IndexDatasetTests, providesResolutionsAsMetadata)
{
	addBlock(0, 0, 2, 2, 1);
	addBlock(0, 0, 4, 2, 2);

	auto** resolutions = getDataset().GetMetadata("Resolutions");

	EXPECT_EQ("1m=", std::string(resolutions[0]));
	EXPECT_EQ("2m=", std::string(resolutions[1]));
	EXPECT_EQ(nullptr, resolutions[2]);
}

TEST_F(IndexDatasetTests, providesARasterBandForEachResolution)
{
	addBlock(0, 0, 10, 10, 5);
	addBlock(0, 0, 10, 10, 2);
	addBlock(0, 0, 10, 10, 1);

	auto& ds = getDataset();

	EXPECT_THAT(ds.getResolutions(), testing::ElementsAre(1, 2, 5));
	EXPECT_EQ(3, ds.GetRasterCount());
}

TEST_F(IndexDatasetTests, providesClutterCodes)
{
	addClutter(0, "sea");
	addClutter(1, "rural");

	auto& actual = getDataset().getClutterCodes();

	ASSERT_EQ(true, actual.is_initialized());
	EXPECT_EQ(2, actual->getNrOfClutterCodes());
}

TEST_F(IndexDatasetTests, setsGeoTransformAccordingToBoundingBox)
{
	addBlock(1, 3, 3, 4, 1);
	addBlock(3, 4, 4, 6, 1);

	double actual[6];
	getDataset().GetGeoTransform(actual);

	double expected[6];
	expected[0] = 1; // east min
	expected[1] = 1; // resolution is always 1
	expected[2] = 0;
	expected[3] = 3; // north min
	expected[4] = 0;
	expected[5] = 1; // resolution is always 1

	ASSERT_THAT(actual, ::testing::ElementsAreArray(expected));
}

TEST_F(IndexDatasetTests, rasterIO)
{
	builder.addBlock().from(0, 0).to(4, 4).resolution(2).withData(
		{ 0, 1, // top-down!
		  2, 3 });
	builder.addBlock().from(2, 0).to(4, 2).resolution(1).withData(
		{ 10, 12,
		  14, 16 });

	// whole bounding box with resolution = 2
	auto pixels = readPixels(makeBox(0, 0, 4, 4), 2, 2, GDALRIOResampleAlg::GRIORA_Bilinear);
	EXPECT_THAT(pixels, testing::ElementsAre(
		2, 13, // bottom-up
		0, 1));

	// a region with resolution = 1
	pixels = readPixels(makeBox(2, 1, 4, 4), 2, 3, GDALRIOResampleAlg::GRIORA_NearestNeighbour);
	EXPECT_THAT(pixels, testing::ElementsAre(
		 10, 12,
		  1,  1,
		  1,  1));
}


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
	setHeader("asdkl�fj�asldkfj");

	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetIdentifyTests, IdentificationWorksIfFirstLineCanBeParsed)
{
	setHeader("file 0 1 0 1 1 \n garbage");

	EXPECT_TRUE(identify());
}
