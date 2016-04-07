#include "GeoParams.h"

using boost::property_tree::wptree;

namespace aircom { namespace pred_raster {

namespace {

MapBox parseBoundingBox(const wptree& geoNode)
{
	MapPoint bottomLeft(geoNode.get<int>(L"left"), geoNode.get<int>(L"bottom"));
	MapPoint topRight(geoNode.get<int>(L"right"), geoNode.get<int>(L"top"));
	return MapBox(bottomLeft, topRight);
}

}

GeoParams::GeoParams(const wptree& geoNode)
	: boundingBox(parseBoundingBox(geoNode))
{}

}}