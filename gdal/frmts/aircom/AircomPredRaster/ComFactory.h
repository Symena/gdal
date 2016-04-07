#pragma once

#include "API.h"

#include <string>

namespace aircom { namespace pred_raster {

class ComFactory
{
public:
	ComFactory(const std::wstring& predAccessClassIDString, const std::wstring& predRasterClassIDString);

	IAircomPredAccess4Ptr createPredAccess() const;
	IPredRaster5Ptr createPredRaster() const;

private:
	CLSID predAccessClassID;
	CLSID predRasterClassID;
};

}}
