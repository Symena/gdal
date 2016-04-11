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

	MapPoint bottomLeft(region.m_eastMin, region.m_northMax - region.m_height);
	MapPoint topRight(region.m_eastMin + region.m_width, region.m_northMax);

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
