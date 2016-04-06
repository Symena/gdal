#include "AircomMap/RasterBand.h"

#include "BlocksBuilder.h"

#include <gmock/gmock.h>

namespace aircom { namespace map {

TEST(RasterBand, constructor)
{
	Dataset dataset(Blocks(), nullptr, "");
	RasterBand band(&dataset, 666);

	EXPECT_EQ(666, band.GetBand());
	EXPECT_EQ(GDALDataType::GDT_Int16, band.GetRasterDataType());
	EXPECT_EQ(-9999, band.GetNoDataValue());

	EXPECT_EQ(GDALColorInterp::GCI_GrayIndex, band.GetColorInterpretation());
}

TEST(RasterBand, clutterNames)
{
	Dataset heightDataset(Blocks(), nullptr, "");
	RasterBand heightBand(&heightDataset);

	EXPECT_EQ(nullptr, heightBand.GetCategoryNames());

	auto clutterFile = std::make_unique<std::stringstream>();
	*clutterFile << "0 sea\n1 rural\n";

	Dataset clutterDataset(Blocks(), std::move(clutterFile), "");
	RasterBand clutterBand(&clutterDataset);
	auto** names = clutterBand.GetCategoryNames();

	ASSERT_NE(nullptr, names);
	EXPECT_EQ("sea", std::string(names[0]));
	EXPECT_EQ("rural", std::string(names[1]));
	EXPECT_EQ(nullptr, names[2]);
}

TEST(RasterBand, rasterIO)
{
	BlocksBuilder builder;
	builder.addBlock().from(0, 0).to(4, 4).resolution(2).withData(
		{ 0, 1, // top-down!
		  2, 3 });
	builder.addBlock().from(2, 0).to(4, 2).resolution(1).withData(
		{ 10, 12,
		  14, 16 });

	Dataset dataset(builder.create(), nullptr, "");
	auto& band = *dataset.GetRasterBand(1);

	auto readPixels = [&band](const MapBox& sourceRegion, int widthInPixels, int heightInPixels)
	{
		std::vector<std::int16_t> pixels(static_cast<size_t>(widthInPixels) * heightInPixels);

		auto error = band.RasterIO(GDALRWFlag::GF_Read,
			sourceRegion.min_corner().get<0>(), sourceRegion.min_corner().get<1>(),
			width(sourceRegion), height(sourceRegion),
			pixels.data(), widthInPixels, heightInPixels, GDALDataType::GDT_Int16,
			0, 0,
			nullptr);
		EXPECT_EQ(CPLErr::CE_None, error);

		return pixels;
	};

	// whole bounding box with resolution = 2
	auto pixels = readPixels(makeBox(0, 0, 4, 4), 2, 2);
	EXPECT_THAT(pixels, testing::ElementsAre(
		0, 1, // top-down
		2, 13));

	// a region with resolution = 1
	pixels = readPixels(makeBox(2, 1, 4, 4), 2, 3);
	EXPECT_THAT(pixels, testing::ElementsAre(
		1, 1,
		2, 2,
		10, 12));
}

}}
