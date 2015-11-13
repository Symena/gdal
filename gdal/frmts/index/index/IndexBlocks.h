#pragma once

#include <vector>
#include <memory>

#include <boost/geometry/index/rtree.hpp>

#include "IndexLine.h"
#include "IndexGeometry.h"
#include "IndexStreamSource.h"

class IndexBlock
{
	MapBox boundingBox; // in meters

	int widthInPixels;
	int heightInPixels;
	int resolution; // in meters

	std::shared_ptr<IndexStreamSource> dataStream;

	int index;

public:
	IndexBlock(const MapBox& boundingBox, int resolution, std::shared_ptr<IndexStreamSource> dataStream, int index);
	IndexBlock(const IndexLine& line, int index);

	const MapBox& getBoundingBox() const { return boundingBox; }

	int getWidthInPixels() const { return widthInPixels; }
	int getHeightInPixels() const { return heightInPixels; }
	int getResolution() const { return resolution; }

	std::unique_ptr<std::istream> getData(IndexWarnings& warnings) const { return dataStream ? dataStream->getStream(warnings) : nullptr; }

	int getIndex() const { return index; }
};


class IndexBlocks
{
	using TreeEntry = std::pair<MapBox, IndexBlock>;

	boost::geometry::index::rtree<TreeEntry, boost::geometry::index::rstar<16>> blocksTree;
	MapBox boundingBox; // in meters

public:
	IndexBlocks() = default;
	explicit IndexBlocks(const std::vector<IndexLine>& lines);

	const MapBox& getBoundingBox() const { return boundingBox; }

	std::vector<IndexBlock> getIntersectingBlocks(const MapBox& box) const;
};
