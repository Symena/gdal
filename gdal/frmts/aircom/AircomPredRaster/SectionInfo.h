#pragma once

#include "gdal.h"
#include "Geometry.h"

#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

struct SectionInfo
{
	MapBox boundingBox;
	GDALDataType dataType;
	MapPoint tileSizeInPixels;

	SectionInfo(const MapBox& boundingBox, GDALDataType dataType, MapPoint tileSizeInPixels);
	explicit SectionInfo(const boost::property_tree::wptree& sectionNode);

	bool operator==(const SectionInfo& r) const;	
};

std::ostream& operator<< (std::ostream& stream, const SectionInfo& sectionInfo);

}}
