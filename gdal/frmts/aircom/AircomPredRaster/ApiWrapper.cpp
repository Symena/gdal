#include "ApiWrapper.h"

#include "PredRasterFactory.h"

#include <mutex>

namespace aircom { namespace pred_raster {

std::mutex coInitialize_mutex;

std::unordered_map<std::thread::id, bool> ApiWrapper::coInitialized;

ApiWrapper::ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster)
	: params(std::move(apiParams)) 
	, predRaster(predRaster)
{
	auto threadId = std::this_thread::get_id();
	std::lock_guard<std::mutex> lock(coInitialize_mutex);
	if (!coInitialized[threadId])
	{
		CoInitialize(nullptr);
		coInitialized[threadId] = true;
	}
}	

IPredRaster5Ptr ApiWrapper::getPredRaster()
{
	if (!predRaster)
		predRaster = PredRasterFactory::createPredRaster(params);

	return predRaster;
}

GeoParams ApiWrapper::getGeoParams()
{
	_REGIONEX region;
	getPredRaster()->GetRegionEx(0, &region);

	MapPoint topLeft(int(region.m_eastMin / 100), int(region.m_northMax / 100));

	const int res = region.m_resolution / 100;
	MapPoint bottomLeft(topLeft.get<0>(), topLeft.get<1>() - region.m_height * res);
	MapPoint topRight(topLeft.get<0>() + region.m_width * res, topLeft.get<1>());

	return GeoParams(MapBox(bottomLeft, topRight));
}

std::vector<unsigned long> ApiWrapper::getSectionNums()
{
	auto numSections = getPredRaster()->GetNumSections();

	std::vector<unsigned long> sections(numSections);
	getPredRaster()->GetSectionNumList(numSections, sections.data());

	return sections;
}

}}
