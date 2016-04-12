#pragma once

#include "AircomPredRaster/API.h"
#include "UnitTests/shared/DummyIUnknownImpl.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockTileIterator : public DummyIUnknownImpl<IPredRasterTileIterator>
{
	MOCK_METHOD1(raw_GetNextTile, HRESULT(IRasterTile ** ppTile));
	MOCK_METHOD3(raw_GetTile, HRESULT(unsigned int tile_x, unsigned int tile_y, IRasterTile ** ppTile));
	MOCK_METHOD1(raw_GetNumTiles, HRESULT(unsigned int * pNumTiles));
};

}}
