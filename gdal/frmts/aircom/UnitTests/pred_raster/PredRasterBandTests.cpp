#include "AircomPredRaster/Rasterband.h"

#include "MockApiWrapper.h"
#include "MockPredRaster.h"
#include "MockTile.h"
#include "MockTileIterator.h"

#include <gmock/gmock.h>

using namespace testing;

namespace aircom { namespace pred_raster {

struct PredRasterBandTest : public Test
{
	MockTileIterator tileIterator;
	ApiParams apiParams = ApiParams(L"", PredData(), L"", L"", Section::Unspecified);
};

TEST_F(PredRasterBandTest, fillsBlocksWithoutTileWithNoDataValue)
{
	RasterBand band(nullptr, {2, 2}, 1, std::make_shared<ApiWrapper>(apiParams, nullptr),
		0, {GDT_Byte, {2, 2}});

	EXPECT_CALL(tileIterator, raw_GetTile(0, 0, _))
		.WillOnce(Return(E_FAIL));

	std::uint8_t blockData[2*2];
	band.readBlock(&tileIterator, {0, 0}, blockData);

	EXPECT_THAT(blockData, ElementsAre(200, 200, 200, 200));
}

TEST_F(PredRasterBandTest, fillsPartialBlocksCorrectly)
{
	// overall size: 8x7 pixels
	// tile size: 5x5 pixels
	// => last tile at (1,1) is 3x2 pixels

	RasterBand band(nullptr, {8, 7}, 1, std::make_shared<ApiWrapper>(apiParams, nullptr),
		0, {GDT_Float32, {5, 5}});

	MockTile tile;

	EXPECT_CALL(tileIterator, raw_GetTile(1, 1, _))
		.WillOnce(DoAll(SetArgPointee<2>(&tile), Return(S_OK)));

	EXPECT_CALL(tile, raw_GetPixelCount(_))
		.WillOnce(DoAll(SetArgPointee<0>(3*2), Return(S_OK)));

	_REGIONEX tileRegion;
	tileRegion.m_width = 3;
	tileRegion.m_height = 2;

	EXPECT_CALL(tile, raw_GetTileRegion(_))
		.WillOnce(DoAll(SetArgPointee<0>(tileRegion), Return(S_OK)));

	EXPECT_CALL(tile, raw_GetFloatData(3*2, _))
		.WillOnce(Invoke([](unsigned long numPixels, float* data)
		{
			data[0] = 1; data[1] = 2; data[2] = 3;
			data[3] = 4; data[4] = 5; data[5] = 6;
			return S_OK;
		}));

	float blockData[5*5] = { 0 };
	band.readBlock(&tileIterator, {1, 1}, blockData);

	float expectedBlockData[5*5] = {
		1, 2, 3, 0, 0,
		4, 5, 6, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	EXPECT_THAT(blockData, ContainerEq(expectedBlockData));
}

TEST_F(PredRasterBandTest, maskValuesOutsideRadius)
{
	// 3x2 tiles à 4x6 pixels
	// res = 2 => 24x24 m
	// tx => (13, 11)
	MockPredRaster predRaster;
	apiParams.predData.nX_cm = 13 * 100;
	apiParams.predData.nY_cm = 11 * 100;
	apiParams.predData.nResolution_cm = 2 * 100;

	auto apiWrapper = std::make_shared<MockApiWrapper>(apiParams, &predRaster);
	SectionInfo sectionInfo{GDT_Float32, {4, 6}};
	RasterBand band(nullptr, {12, 12}, 1, apiWrapper, 0, sectionInfo);
	Auxiliary auxiliary({{0,0}, {24, 24}}, 0, {{0, sectionInfo}});

	EXPECT_CALL(*apiWrapper, getAuxiliary())
		.WillOnce(Return(auxiliary));

	EXPECT_CALL(predRaster, raw_CreateTileIterator(0, _))
		.WillRepeatedly(DoAll(SetArgPointee<1>(&tileIterator), Return(S_OK)));

	MockTile tile;
	std::vector<float> filledTile(4 * 6);
	filledTile.assign(filledTile.size(), 1);

	EXPECT_CALL(tileIterator, raw_GetTile(_, _, _))
		.WillRepeatedly(DoAll(SetArgPointee<2>(&tile), Return(S_OK)));

	EXPECT_CALL(tile, raw_GetPixelCount(_))
		.WillRepeatedly(DoAll(SetArgPointee<0>(filledTile.size()), Return(S_OK)));

	EXPECT_CALL(tile, raw_GetFloatData(filledTile.size(), _))
		.WillRepeatedly(Invoke([&](auto numElements, auto* rasterData) { 
		for (auto i = 0; i < numElements; ++i)
			rasterData[i] = filledTile[i];
		return S_OK; 
	}));
	
	// When
	std::vector<float> actual(12 * 12);
	band.RasterIO(GF_Read, 0, 0, 12, 12, actual.data(), 12, 12, GDT_Float32, 0, 0);

	// Then
	auto noDataValue = band.GetNoDataValue();
	EXPECT_DOUBLE_EQ(-9999, noDataValue);
	EXPECT_EQ(noDataValue, actual[0]);
	EXPECT_EQ(1, actual[actual.size() / 2]);
	EXPECT_EQ(noDataValue, actual[actual.size() - 1]);


}

}}
