#pragma once

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

using MapPoint = boost::geometry::model::point<int, 2, boost::geometry::cs::cartesian>;
using MapBox = boost::geometry::model::box<MapPoint>;

namespace boost { namespace geometry { namespace model {

inline int width(const MapBox& box)
{
	return box.max_corner().get<0>() - box.min_corner().get<0>();
}

inline int height(const MapBox& box)
{
	return box.max_corner().get<1>() - box.min_corner().get<1>();
}

inline MapPoint upper_left(const MapBox& box)
{
	return {box.min_corner().get<0>(), box.max_corner().get<1>()};
}

inline MapPoint lower_right(const MapBox& box)
{
	return {box.max_corner().get<0>(), box.min_corner().get<1>()};
}

inline MapPoint operator+(MapPoint lhs, MapPoint rhs)
{
	return { lhs.get<0>() + rhs.get<0>(), lhs.get<1>() + rhs.get<1>() };
}

inline MapPoint operator-(MapPoint lhs, MapPoint rhs)
{
	return { lhs.get<0>() - rhs.get<0>(), lhs.get<1>() - rhs.get<1>() };
}

inline bool operator==(const MapBox& lhs, const MapBox& rhs)
{
	return lhs.min_corner().get<0>() == rhs.min_corner().get<0>()
		&& lhs.min_corner().get<1>() == rhs.min_corner().get<1>()
		&& lhs.max_corner().get<0>() == rhs.max_corner().get<0>()
		&& lhs.max_corner().get<1>() == rhs.max_corner().get<1>();
}

}}}

inline MapBox makeBox(int minX, int minY, int maxX, int maxY)
{
	return { MapPoint(minX, minY), MapPoint(maxX, maxY) };
}
