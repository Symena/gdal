#include "PredRasterFactory.h"

#include "StringUtils.h"

namespace aircom { namespace pred_raster {

namespace {

template <typename Ptr>
Ptr createComInstance(const CLSID& classID, const char* interfaceName)
{
	Ptr ptr;
	if (SUCCEEDED(ptr.CreateInstance(classID)))
		return ptr;

	LPOLESTR classIDString = L"";
	StringFromCLSID(classID, &classIDString);

	const auto msg = format("Failed to create COM interface %s using COM class ID %s",
		interfaceName, classIDString);

	throw std::runtime_error(msg);
}

std::string getPredAccessStatusText(IAircomPredAccess4Ptr predAccess, unsigned short statusCode)
{
	_bstr_t statusText;
	predAccess->GetStatusText(statusCode, statusText.GetAddress());
	return static_cast<const char*>(statusText);
}

}

bool PredRasterFactory::PredAccessKey::operator<(const PredAccessKey& r) const
{
	const auto classIDResult = memcmp(&classID, &r.classID, sizeof(CLSID));
	if (classIDResult != 0)
		return classIDResult < 0;

	return predictionsFolder < r.predictionsFolder;
}


std::mutex PredRasterFactory::mutex;
std::map<PredRasterFactory::PredAccessKey, IAircomPredAccess4Ptr> PredRasterFactory::predAccessMap;

CLSID PredRasterFactory::getClassID(const std::wstring& idString)
{
	CLSID classID;
	if (FAILED(CLSIDFromString(idString.c_str(), &classID)))
		throw std::runtime_error(format("Invalid COM class ID %s", idString));
	return classID;
}

IAircomPredAccess4Ptr PredRasterFactory::getPredAccess(const ApiParams& params)
{
	std::lock_guard<std::mutex> lock(mutex);

	const PredAccessKey key = { getClassID(params.predAccessClassID), params.predictionFolder };

	auto it = predAccessMap.find(key);
	if (it != predAccessMap.end())
		return it->second;

	auto predAccess = createPredAccess(key);
	predAccessMap.emplace(key, predAccess);

	return predAccess;
}

IAircomPredAccess4Ptr PredRasterFactory::createPredAccess(const PredAccessKey& key)
{
	auto predAccess = createComInstance<IAircomPredAccess4Ptr>(key.classID, "IAircomPredAccess4");

	const _bstr_t predictionsFolder(key.predictionsFolder.wstring().c_str());

	constexpr unsigned short retryCount = 2;
	unsigned short statusCode;
	predAccess->Initialise(predictionsFolder, _bstr_t(), retryCount, &statusCode);

	if (statusCode != 0)
	{
		throw std::runtime_error(format("Failed to initialize ASSET prediction access: %s (%d)",
			getPredAccessStatusText(predAccess, statusCode), statusCode));
	}

	return predAccess;
}

IPredRaster5Ptr PredRasterFactory::createPredRaster(const ApiParams& params)
{
	auto predAccess = getPredAccess(params);

	// lock this operation due to Asset thread-safety issues
	static std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);

	auto paramsCopy = params;
	unsigned long rasterHandle;
	unsigned short statusCode;
	predAccess->ExtractPathlossPredictionRaster(&paramsCopy.predData, &rasterHandle, &statusCode);

	if (statusCode != 0)
	{
		throw std::runtime_error(format("Failed to extract pathloss prediction raster: %s (%d)",
			getPredAccessStatusText(predAccess, statusCode), statusCode));
	}

	auto predRaster = createComInstance<IPredRaster5Ptr>(getClassID(params.predRasterClassID), "IPredRaster5");
	predRaster->Attach(rasterHandle);

	return predRaster;
}

}}
