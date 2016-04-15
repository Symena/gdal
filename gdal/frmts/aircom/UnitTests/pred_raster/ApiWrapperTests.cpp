#include "AircomPredRaster/ApiWrapper.h"

#include "MockApiWrapper.h"
#include "MockPredRaster.h"
#include "MockTile.h"
#include "MockTileIterator.h"

using namespace testing;

namespace aircom { namespace pred_raster {

TEST(ApiWrapper, getAuxiliary)
{
	ApiParams params("z:/ombie", PredData(), L"123-456", L"567-890", Section::Unspecified);
	MockPredRaster predRaster;
	ApiWrapper apiWrapper(params, &predRaster);

	// section numbers
	EXPECT_CALL(predRaster, raw_GetNumSections(_))
		.WillOnce(DoAll(SetArgPointee<0>(2), Return(S_OK)));
	EXPECT_CALL(predRaster, raw_GetSectionNumList(2, _))
		.WillOnce(Invoke([](auto numSections, unsigned long* sectionNums)
		{
			sectionNums[0] = 1;
			sectionNums[1] = 3;
			return S_OK;
		}));

	// section data types
	EXPECT_CALL(predRaster, raw_GetSectionDataType(1, _))
		.WillOnce(DoAll(SetArgPointee<1>(RasterSectionType_Double), Return(S_OK)));
	EXPECT_CALL(predRaster, raw_GetSectionDataType(3, _))
		.WillOnce(DoAll(SetArgPointee<1>(RasterSectionType_UnsignedChar), Return(S_OK)));

	// section tile sizes

	MockTileIterator section1Iterator, section3Iterator;
	MockTile section1FirstTile, section3FirstTile;
	_REGIONEX section1FirstTileRegion, section3FirstTileRegion;

	section1FirstTileRegion.m_width = 512;
	section1FirstTileRegion.m_height = 256;
	EXPECT_CALL(predRaster, raw_CreateTileIterator(1, _))
		.WillOnce(DoAll(SetArgPointee<1>(&section1Iterator), Return(S_OK)));
	EXPECT_CALL(section1Iterator, raw_GetNextTile(_))
		.WillOnce(DoAll(SetArgPointee<0>(&section1FirstTile), Return(S_OK)));
	EXPECT_CALL(section1FirstTile, raw_GetTileRegion(_))
		.WillOnce(DoAll(SetArgPointee<0>(section1FirstTileRegion), Return(S_OK)));

	section3FirstTileRegion.m_width = 720;
	section3FirstTileRegion.m_height = 360;
	EXPECT_CALL(predRaster, raw_CreateTileIterator(3, _))
		.WillOnce(DoAll(SetArgPointee<1>(&section3Iterator), Return(S_OK)));
	EXPECT_CALL(section3Iterator, raw_GetNextTile(_))
		.WillOnce(DoAll(SetArgPointee<0>(&section3FirstTile), Return(S_OK)));
	EXPECT_CALL(section3FirstTile, raw_GetTileRegion(_))
		.WillOnce(DoAll(SetArgPointee<0>(section3FirstTileRegion), Return(S_OK)));

	// first section's bounding box & EPSG

	_REGIONEX r;
	r.m_eastMin = 100;
	r.m_northMax = 200;
	r.m_resolution = 300;
	r.m_width = 4;
	r.m_height = 5;
	r.m_EPSG = 666;

	EXPECT_CALL(predRaster, raw_GetRegionEx(1, _))
		.WillOnce(DoAll(SetArgPointee<1>(r), Return(S_OK)));

	const auto actual = apiWrapper.getAuxiliary();

	const auto expectedBounds = makeBox(int(r.m_eastMin) / 100, int(r.m_northMax - r.m_height * r.m_resolution) / 100,
	                                    int(r.m_eastMin + r.m_width * r.m_resolution) / 100, int(r.m_northMax) / 100);
	Auxiliary expected(expectedBounds, r.m_EPSG, {
		{ 1, {GDT_Float64, {512, 256}} },
		{ 3, {GDT_Byte,    {720, 360}} }
	});

	EXPECT_EQ(expected, actual);
}

}}
