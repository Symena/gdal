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
	MockPredRaster predRaster;
	MockTileIterator tileIterator;
	ApiParams apiParams = ApiParams(L"", PredData(), L"", L"", Section::Unspecified);
};

TEST_F(PredRasterBandTest, fillsBlocksWithoutTileWithNoDataValue)
{
	RasterBand band(nullptr, {2, 2}, 1, std::make_shared<ApiWrapper>(apiParams, &predRaster),
		0, {GDT_Byte, {2, 2}});

	EXPECT_CALL(predRaster, raw_CreateTileIterator(0, _))
		.WillOnce(DoAll(SetArgPointee<1>(&tileIterator), Return(S_OK)));

	EXPECT_CALL(tileIterator, raw_GetTile(0, 0, _))
		.WillOnce(Return(E_FAIL));

	std::uint8_t blockData[2*2];
	band.ReadBlock(0, 0, blockData);

	EXPECT_THAT(blockData, ElementsAre(200, 200, 200, 200));
}

TEST_F(PredRasterBandTest, fillsPartialBlocksCorrectly)
{
	// overall size: 8x7 pixels
	// tile size: 5x5 pixels
	// => last tile at (1,1) is 3x2 pixels

	RasterBand band(nullptr, {8, 7}, 1, std::make_shared<ApiWrapper>(apiParams, &predRaster),
		0, {GDT_Float32, {5, 5}});

	MockTile tile;

	EXPECT_CALL(predRaster, raw_CreateTileIterator(0, _))
		.WillOnce(DoAll(SetArgPointee<1>(&tileIterator), Return(S_OK)));

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
	band.ReadBlock(1, 1, blockData);

	float expectedBlockData[5*5] = {
		1, 2, 3, 0, 0,
		4, 5, 6, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	EXPECT_THAT(blockData, ContainerEq(expectedBlockData));
}

}}
