#pragma once

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <ostream>

namespace aircom {

using Point = boost::geometry::model::d2::point_xy<int>;
using Rectangle = boost::geometry::model::box<Point>;

inline Rectangle makeRectangle(int minX, int minY, int maxX, int maxY)
{
	return { Point(minX, minY), Point(maxX, maxY) };
}

}

namespace boost { namespace geometry { namespace model {

inline int width(const aircom::Rectangle& box)
{
	return box.max_corner().x() - box.min_corner().x();
}

inline int height(const aircom::Rectangle& box)
{
	return box.max_corner().y() - box.min_corner().y();
}

inline aircom::Point upper_left(const aircom::Rectangle& box)
{
	return {box.min_corner().x(), box.max_corner().y()};
}

inline aircom::Point lower_right(const aircom::Rectangle& box)
{
	return {box.max_corner().x(), box.min_corner().y()};
}

inline aircom::Point operator+(aircom::Point lhs, aircom::Point rhs)
{
	return { lhs.x() + rhs.x(), lhs.y() + rhs.y() };
}

inline aircom::Point operator-(aircom::Point lhs, aircom::Point rhs)
{
	return { lhs.x() - rhs.x(), lhs.y() - rhs.y() };
}

inline bool operator==(const aircom::Point& lhs, const aircom::Point& rhs)
{
	return lhs.x() == rhs.x()
		&& lhs.y() == rhs.y();
}

inline bool operator==(const aircom::Rectangle& lhs, const aircom::Rectangle& rhs)
{
	return lhs.min_corner() == rhs.min_corner()
		&& lhs.max_corner() == rhs.max_corner();
}

inline std::ostream& operator<<(std::ostream& stream, const aircom::Point& p)
{
	stream << "{ x: " << p.x() << ", y: " << p.y() << " }";
	return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const aircom::Rectangle& b)
{
	stream << "{ BL: " << b.min_corner() << ", TR: " << b.max_corner() << " }";
	return stream;
}

}}}

