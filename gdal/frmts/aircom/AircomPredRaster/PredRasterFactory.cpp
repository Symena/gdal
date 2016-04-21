#include "PredRasterFactory.h"

#include "StringUtils.h"

#include <boost/filesystem/operations.hpp>
#include <map>
#include <mutex>
#include <thread>
#include <unordered_set>

namespace aircom { namespace pred_raster {

namespace {

template <typename Ptr>
Ptr createComInstance(const CLSID& classID, const char* interfaceName)
{
	Ptr ptr;
	if (SUCCEEDED(ptr.CreateInstance(classID)))
		return ptr;

	LPOLESTR classIDString = L"";
	const auto hr = StringFromCLSID(classID, &classIDString);

	const auto msg = format("Failed to create COM interface %s using COM class ID %s",
		interfaceName, classIDString);

	if (hr == S_OK)
		CoTaskMemFree(classIDString);

	throw std::runtime_error(msg);
}

std::string getPredAccessStatusText(IAircomPredAccess4Ptr predAccess, unsigned short statusCode)
{
	_bstr_t statusText;
	predAccess->GetStatusText(statusCode, statusText.GetAddress());
	return static_cast<const char*>(statusText);
}

std::mutex mutex;
std::unordered_set<std::thread::id> initializedThreads;
std::map<PredRasterFactory::PredAccessKey, IAircomPredAccess4Ptr> predAccessMap;

}

bool PredRasterFactory::PredAccessKey::operator<(const PredAccessKey& r) const
{
	const auto classIDResult = memcmp(&classID, &r.classID, sizeof(CLSID));
	if (classIDResult != 0)
		return classIDResult < 0;

	if (boost::filesystem::equivalent(predictionsFolder, r.predictionsFolder))
		return false;

	return predictionsFolder < r.predictionsFolder;
}


CLSID PredRasterFactory::getClassID(const std::wstring& idString)
{
	CLSID classID;
	if (FAILED(CLSIDFromString(idString.c_str(), &classID)))
		throw std::runtime_error(format("Invalid COM class ID %s", idString));
	return classID;
}

IAircomPredAccess4Ptr PredRasterFactory::getPredAccess(const ApiParams& params)
{
	const auto threadID = std::this_thread::get_id();
	const PredAccessKey key = { getClassID(params.predAccessClassID), params.predictionFolder };

	std::lock_guard<std::mutex> lock(mutex);

	// Make sure the COM system is initialized for this thread before attempting
	// to create COM instances.
	// We don't use CoUninitialize to prevent invalidating cached PredAccess
	// instances.
	if (initializedThreads.find(threadID) == initializedThreads.end())
	{
		CoInitialize(nullptr);
		initializedThreads.insert(threadID);
	}

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
	unsigned short statusCode = 0; // must be preinitialized - the API doesn't set it on success!
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

	auto mutablePredData = params.predData;
	unsigned long rasterHandle;
	unsigned short statusCode = 0;
	predAccess->ExtractPathlossPredictionRaster(&mutablePredData, &rasterHandle, &statusCode);

	if (statusCode != 0)
	{
		throw std::runtime_error(format("Failed to extract pathloss prediction raster: %s (%d)",
			getPredAccessStatusText(predAccess, statusCode), statusCode));
	}

	auto predRaster = createComInstance<IPredRaster5Ptr>(getClassID(params.predRasterClassID), "IPredRaster5");
	predRaster->Attach(rasterHandle);

	return predRaster;
}

void PredRasterFactory::cleanup()
{
	std::lock_guard<std::mutex> lock(mutex);
	predAccessMap.clear();
}

}}
