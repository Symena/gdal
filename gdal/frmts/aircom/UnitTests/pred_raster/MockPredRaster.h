#pragma once

#include "AircomPredRaster/API.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockPredRaster : public IPredRaster5Ptr
{
	MOCK_METHOD2(raw_GetRegionEx, HRESULT(unsigned long nSectionNumber, struct _REGIONEX * pRegion));
};

}}