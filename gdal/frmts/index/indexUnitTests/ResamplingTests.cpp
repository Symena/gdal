#include "Resampling.h"

#include <gmock/gmock.h>

TEST(Resampling, bilinearDownsampling)
{
	// 2x2 pixels => single pixel
	int src[] = {
		10, 20,
		30, 40 };

	int dst;
	resample(src, 2, 2, &dst, 1, 1, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Bilinear);

	EXPECT_EQ(25, dst);
}

TEST(Resampling, nearestNeighborUpsampling)
{
	// 2x1 pixels => 4x2 pixels
	int src[] = { 10, 20 };

	int dst[8];
	resample(src, 2, 1, dst, 4, 2, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_NearestNeighbour);

	EXPECT_THAT(dst, testing::ElementsAre(
		10, 10,   20, 20,
		10, 10,   20, 20));
}

TEST(Resampling, majorityDownsampling_withNoDataValue)
{
	// 4x4 pixels => 2x2 pixels
	int src[] = {
		99,  3,   10, 99,
		99, 99,   12, 10,

		99, 99,   99, 99,
		 1, 99,   99, 99 };

	int dst[4];
	resample(src, 4, 4, &dst, 2, 2, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Mode, 99);

	EXPECT_THAT(dst, testing::ElementsAre(
		3, 10,
		1, 99));
}

TEST(Resampling, bilinearUpsampling_withNoDataValue)
{
	// 2x2 pixels => 4x4 pixels
	int src[] = {
		100,  99,
		 99, 200 };

	int dst[16];
	resample(src, 2, 2, &dst, 4, 4, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Bilinear, 99);

	EXPECT_THAT(dst, testing::ElementsAreArray(std::vector<int>{
		100, 100,              100,  99,
		100, 125,              125 /* 150? */, 200,

		100, 175 /* 150? */,   175, 200,
		 99, 200,              200, 200 }));
}

TEST(Resampling, bilinearUpsampling_onlyNoDataValues)
{
	// 2x2 pixels => 4x4 pixels
	int src[] = {
		99, 99,
		99, 99 };

	int dst[16];
	resample(src, 2, 2, &dst, 4, 4, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Bilinear, 99);

	EXPECT_THAT(dst, testing::ElementsAreArray(std::vector<int>{
		99, 99,   99, 99,
		99, 99,   99, 99,

		99, 99,   99, 99,
		99, 99,   99, 99 }));
}

TEST(Resampling, bilinearDownsampling_withNoDataValue)
{
	// 4x4 pixels => 2x2 pixels
	int src[] = {
		  99,  200,    500,   99,
		 300,   99,     99,  800,
	
		1100, 1200,     99,   99,
		1300, 1400,     99,   99 };

	int dst[4];
	resample(src, 4, 4, &dst, 2, 2, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Bilinear, 99);

	// interesting results...
	EXPECT_THAT(dst, testing::ElementsAre(
		 411,  696,
		1114, 1229));
}

TEST(Resampling, bilinearDownsampling_onlyNoDataValues)
{
	// 4x4 pixels => 2x2 pixels
	int src[] = {
		99, 99,   99, 99,
		99, 99,   99, 99,

		99, 99,   99, 99,
		99, 99,   99, 99 };

	int dst[4];
	resample(src, 4, 4, &dst, 2, 2, GDALDataType::GDT_Int32, GDALRIOResampleAlg::GRIORA_Bilinear, 99);

	EXPECT_THAT(dst, testing::ElementsAre(
		99, 99,
		99, 99));
}
