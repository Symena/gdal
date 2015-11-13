#include "IndexBlocks.h"

#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/range/algorithm/sort.hpp>

IndexBlock::IndexBlock(const MapBox& boundingBox, int resolution, std::shared_ptr<IndexStreamSource> dataStream, int index)
	: boundingBox(boundingBox)
	, widthInPixels(width(boundingBox) / resolution)
	, heightInPixels(height(boundingBox) / resolution)
	, resolution(resolution)
	, dataStream(std::move(dataStream))
	, index(index)
{
	assert(widthInPixels * resolution == width(boundingBox));
	assert(heightInPixels * resolution == height(boundingBox));
}

IndexBlock::IndexBlock(const IndexLine& line, int index)
	: IndexBlock(makeBox(line.getTileEastMin(), line.getTileNorthMin(), line.getTileEastMax(), line.getTileNorthMax()),
	             line.getResolution(), line.getTileDataSource(), index)
{}



static_assert(std::is_move_constructible<IndexBlocks>::value, "IndexBlocks should be move constructible");
static_assert(std::is_move_assignable<IndexBlocks>::value, "IndexBlocks should be move assignable");

IndexBlocks::IndexBlocks(const std::vector<IndexLine>& lines)
{
	int lineNr = 0;
	for (const auto& line : lines)
	{
		auto block = IndexBlock(line, lineNr++);
		blocksTree.insert(std::make_pair(block.getBoundingBox(), block));
	}

	boundingBox = (!blocksTree.empty() ? blocksTree.bounds() : makeBox(0, 0, 0, 0));
}

std::vector<IndexBlock> IndexBlocks::getIntersectingBlocks(const MapBox& box) const
{
	auto isNotOnlyBorderMatch = [&box](const TreeEntry& entry)
	{
		MapBox intersectionBox;
		boost::geometry::intersection(entry.first, box, intersectionBox);

		auto intersectionArea = boost::geometry::area(intersectionBox);
		return intersectionArea != 0;
	};

	std::vector<TreeEntry> intersectingEntries;
	blocksTree.query(boost::geometry::index::intersects(box)
	                 && boost::geometry::index::satisfies(isNotOnlyBorderMatch),
	                 std::back_inserter(intersectingEntries));

	std::vector<IndexBlock> result;
	result.reserve(intersectingEntries.size());
	for (auto& entry : intersectingEntries)
		result.push_back(std::move(entry.second));

	auto comparer = [](const auto& lhs, const auto& rhs)
	{
		if (lhs.getResolution() != rhs.getResolution())
			return lhs.getResolution() > rhs.getResolution(); // coarser resolutions first
		return lhs.getIndex() < rhs.getIndex(); // by line order in index file for identical resolutions
	};

	boost::range::sort(result, comparer);

	return result;
}
