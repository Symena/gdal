#pragma once

#include "API.h"
#include "ApiParams.h"

#include <string>

namespace aircom { namespace pred_raster {

class PredRasterFactory
{
public:
	struct PredAccessKey
	{
		CLSID classID;
		boost::filesystem::path predictionsFolder;

		PredAccessKey(CLSID classID, const boost::filesystem::path& predictionsFolder);

		bool operator<(const PredAccessKey& r) const;
	};

	static IPredRaster5Ptr createPredRaster(const ApiParams& params);

	static void cleanup();

private:
	static CLSID getClassID(const std::wstring& idString);

	static IAircomPredAccess4Ptr getPredAccess(const ApiParams& params);
	static IAircomPredAccess4Ptr createPredAccess(const PredAccessKey& key);
};

}}
