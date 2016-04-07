#pragma once

#include "API.h"
#include "ApiParams.h"

#include <map>
#include <mutex>
#include <string>

namespace aircom { namespace pred_raster {

class PredRasterFactory
{
public:
	static IPredRaster5Ptr createPredRaster(const ApiParams& params);

private:
	struct PredAccessKey
	{
		CLSID classID;
		boost::filesystem::path predictionsFolder;

		bool operator<(const PredAccessKey& r) const;
	};

	static std::mutex mutex;
	static std::map<PredAccessKey, IAircomPredAccess4Ptr> predAccessMap;

	static CLSID getClassID(const std::wstring& idString);

	static IAircomPredAccess4Ptr getPredAccess(const ApiParams& params);
	static IAircomPredAccess4Ptr createPredAccess(const PredAccessKey& key);
};

}}
