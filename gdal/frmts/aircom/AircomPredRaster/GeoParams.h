#pragma once

#include "Geometry.h"

#include <boost/property_tree/ptree.hpp>


namespace aircom { namespace pred_raster {

struct GeoParams
{
	MapBox boundingBox;

	explicit GeoParams(const MapBox& boundingBox)
		: boundingBox(boundingBox)
	{}

	explicit GeoParams(const boost::property_tree::wptree& geoNode);
};

}}