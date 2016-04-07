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
	// TODO
	return GeoParams(MapBox());
}

}}
