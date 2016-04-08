#include "ApiWrapper.h"

#include "PredRasterFactory.h"

namespace aircom { namespace pred_raster {

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

const std::vector<unsigned long>& ApiWrapper::getSectionNums()
{
	if (!sections)
	{
		sections.emplace();
		auto numSections = getPredRaster()->GetNumSections();
		sections->resize(numSections);
		getPredRaster()->GetSectionNumList(numSections, &sections.get()[0]);
	}
	return sections.get();
}

}}
