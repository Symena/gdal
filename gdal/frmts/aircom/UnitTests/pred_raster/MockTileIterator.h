#pragma once

#include "AircomPredRaster/API.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockTileIterator : public IPredRasterTileIterator
{
	// Inherited via IPredRasterTileIterator
	virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) override {
		(*ppvObject) = this;
		return S_OK;
	}

	MOCK_METHOD0(AddRef, ULONG(void));
	MOCK_METHOD0(Release, ULONG (void));

	// Inherited via 
	MOCK_METHOD1(raw_GetNextTile, HRESULT(IRasterTile ** ppTile));
	MOCK_METHOD3(raw_GetTile, HRESULT(unsigned int tile_x, unsigned int tile_y, IRasterTile ** ppTile));
	MOCK_METHOD1(raw_GetNumTiles, HRESULT(unsigned int * pNumTiles));
};

}}
