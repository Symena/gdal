#pragma once

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

using MapPoint = boost::geometry::model::point<int, 2, boost::geometry::cs::cartesian>;
using MapBox = boost::geometry::model::box<MapPoint>;

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