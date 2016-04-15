#pragma once

#include "gdal.h"
#include "Geometry.h"

#include <boost/property_tree/ptree.hpp>
#include <map>

namespace aircom { namespace pred_raster {

struct SectionInfo
{
	GDALDataType dataType;
	MapPoint tileSizeInPixels;

	bool operator==(const SectionInfo& r) const
	{
		return dataType == r.dataType && tileSizeInPixels == r.tileSizeInPixels;
	}
};

using SectionInfos = std::map<unsigned long, SectionInfo>; // sectionNum => SectionInfo

struct Auxiliary
{
	MapBox boundingBox;
	int epsg;
	SectionInfos sectionInfos;

	Auxiliary(const MapBox& boundingBox, int epsg, SectionInfos sectionInfos);
	explicit Auxiliary(const boost::property_tree::wptree& auxiliaryNode);

	boost::property_tree::wptree asPropertyTree() const;

	bool operator==(const Auxiliary& r) const;
};

std::ostream& operator<< (std::ostream& stream, const Auxiliary& auxiliary);

}}
