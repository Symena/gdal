#pragma once

#include "AircomPredRaster/API.h"
#include "UnitTests/shared/DummyIUnknownImpl.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockTile : public DummyIUnknownImpl<IRasterTile>
{
	MOCK_METHOD1(raw_GetTileRegion, HRESULT(_REGIONEX * pRegion));
	MOCK_METHOD1(raw_GetPixelCount, HRESULT(unsigned __int64 * pNumPix));
	MOCK_METHOD2(raw_GetCharData, HRESULT(unsigned long numElements, char * pRasterData));
	MOCK_METHOD2(raw_GetUCharData, HRESULT(unsigned long numElements, unsigned char * pRasterData));
	MOCK_METHOD2(raw_GetShortData, HRESULT(unsigned long numElements, short * pRasterData));
	MOCK_METHOD2(raw_GetUShortData, HRESULT(unsigned long numElements, unsigned short * pRasterData));
	MOCK_METHOD2(raw_GetIntData, HRESULT(unsigned long numElements, int * pRasterData));
	MOCK_METHOD2(raw_GetUIntData, HRESULT(unsigned long numElements, unsigned int * pRasterData));
	MOCK_METHOD2(raw_GetInt64Data, HRESULT(unsigned long numElements, __int64 * pRasterData));
	MOCK_METHOD2(raw_GetUInt64Data, HRESULT(unsigned long numElements, unsigned __int64 * pRasterData));
	MOCK_METHOD2(raw_GetFloatData, HRESULT(unsigned long numElements, float * pRasterData));
	MOCK_METHOD2(raw_GetDoubleData, HRESULT(unsigned long numElements, double * pRasterData));
};

}}
