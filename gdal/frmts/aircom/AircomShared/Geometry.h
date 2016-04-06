#pragma once

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace aircom_map {

using MapPoint = boost::geometry::model::point<int, 2, boost::geometry::cs::cartesian>;
using MapBox = boost::geometry::model::box<MapPoint>;

inline MapBox makeBox(int minX, int minY, int maxX, int maxY)
{
	return { MapPoint(minX, minY), MapPoint(maxX, maxY) };
}

}

namespace boost { namespace geometry { namespace model {

inline int width(const aircom_map::MapBox& box)
{
	return box.max_corner().get<0>() - box.min_corner().get<0>();
}

inline int height(const aircom_map::MapBox& box)
{
	return box.max_corner().get<1>() - box.min_corner().get<1>();
}

inline aircom_map::MapPoint upper_left(const aircom_map::MapBox& box)
{
	return {box.min_corner().get<0>(), box.max_corner().get<1>()};
}

inline aircom_map::MapPoint lower_right(const aircom_map::MapBox& box)
{
	return {box.max_corner().get<0>(), box.min_corner().get<1>()};
}

inline aircom_map::MapPoint operator+(aircom_map::MapPoint lhs, aircom_map::MapPoint rhs)
{
	return { lhs.get<0>() + rhs.get<0>(), lhs.get<1>() + rhs.get<1>() };
}

inline aircom_map::MapPoint operator-(aircom_map::MapPoint lhs, aircom_map::MapPoint rhs)
{
	return { lhs.get<0>() - rhs.get<0>(), lhs.get<1>() - rhs.get<1>() };
}

inline bool operator==(const aircom_map::MapBox& lhs, const aircom_map::MapBox& rhs)
{
	return lhs.min_corner().get<0>() == rhs.min_corner().get<0>()
		&& lhs.min_corner().get<1>() == rhs.min_corner().get<1>()
		&& lhs.max_corner().get<0>() == rhs.max_corner().get<0>()
		&& lhs.max_corner().get<1>() == rhs.max_corner().get<1>();
}

}}}
