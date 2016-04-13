#pragma once

#include "gdal.h"
#include "Geometry.h"

#include <boost/property_tree/ptree.hpp>
#include <map>

namespace aircom { namespace pred_raster {

struct Auxiliary
{
	MapBox boundingBox;
	std::map<unsigned long, GDALDataType> sectionDataTypes;
	MapPoint tileSizeInPixels;

	Auxiliary(const MapBox& boundingBox, std::map<unsigned long, GDALDataType> dataType, MapPoint tileSizeInPixels);
	explicit Auxiliary(const boost::property_tree::wptree& auxiliaryNode);

	bool operator==(const Auxiliary& r) const;
};

std::ostream& operator<< (std::ostream& stream, const Auxiliary& auxiliary);

}}
