#pragma once

#include <vector>
#include <memory>

#include <boost/geometry/index/rtree.hpp>

#include "Line.h"
#include "Geometry.h"
#include "StreamSource.h"

namespace aircom_map {

class Block
{
	MapBox boundingBox; // in meters

	int widthInPixels;
	int heightInPixels;
	int resolution; // in meters

	int index;

	std::shared_ptr<StreamSource> dataStream;

public:
	Block(const MapBox& boundingBox, int resolution, std::shared_ptr<StreamSource> dataStream, int index);
	Block(const Line& line, int index);

	const MapBox& getBoundingBox() const { return boundingBox; }

	int getWidthInPixels() const { return widthInPixels; }
	int getHeightInPixels() const { return heightInPixels; }
	int getResolution() const { return resolution; }

	std::unique_ptr<std::istream> getData(Warnings& warnings) const { return dataStream ? dataStream->getStream(warnings) : nullptr; }

	int getIndex() const { return index; }
};


class Blocks
{
	using TreeEntry = std::pair<MapBox, Block>;

	boost::geometry::index::rtree<TreeEntry, boost::geometry::index::rstar<16>> blocksTree;
	MapBox boundingBox; // in meters
	std::set<int> resolutions;

public:
	explicit Blocks(const std::vector<Line>& lines = {});

	const MapBox& getBoundingBox() const { return boundingBox; }

	const std::set<int>& getResolutions() const { return resolutions; }

	std::vector<Block> getIntersectingBlocks(const MapBox& box) const;
};

}
