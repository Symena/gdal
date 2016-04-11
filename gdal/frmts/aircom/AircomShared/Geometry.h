#pragma once

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace aircom {

using MapPoint = boost::geometry::model::point<int, 2, boost::geometry::cs::cartesian>;
using MapBox = boost::geometry::model::box<MapPoint>;

inline MapBox makeBox(int minX, int minY, int maxX, int maxY)
{
	return { MapPoint(minX, minY), MapPoint(maxX, maxY) };
}

}

namespace boost { namespace geometry { namespace model {

inline int width(const aircom::MapBox& box)
{
	return box.max_corner().get<0>() - box.min_corner().get<0>();
}

inline int height(const aircom::MapBox& box)
{
	return box.max_corner().get<1>() - box.min_corner().get<1>();
}

inline aircom::MapPoint upper_left(const aircom::MapBox& box)
{
	return {box.min_corner().get<0>(), box.max_corner().get<1>()};
}

inline aircom::MapPoint lower_right(const aircom::MapBox& box)
{
	return {box.max_corner().get<0>(), box.min_corner().get<1>()};
}

inline aircom::MapPoint operator+(aircom::MapPoint lhs, aircom::MapPoint rhs)
{
	return { lhs.get<0>() + rhs.get<0>(), lhs.get<1>() + rhs.get<1>() };
}

inline aircom::MapPoint operator-(aircom::MapPoint lhs, aircom::MapPoint rhs)
{
	return { lhs.get<0>() - rhs.get<0>(), lhs.get<1>() - rhs.get<1>() };
}

inline bool operator==(const aircom::MapPoint& lhs, const aircom::MapPoint& rhs)
{
	return lhs.get<0>() == rhs.get<0>()
		&& lhs.get<1>() == rhs.get<1>();
}

inline bool operator==(const aircom::MapBox& lhs, const aircom::MapBox& rhs)
{
	return lhs.min_corner() == rhs.min_corner()
		&& lhs.max_corner() == rhs.max_corner();
}

}}}
