#include "Blocks.h"

#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace aircom { namespace map {

Block::Block(const Rectangle& boundingBox, int resolution, std::shared_ptr<StreamSource> dataStream, int index)
	: boundingBox(boundingBox)
	, widthInPixels(width(boundingBox) / resolution)
	, heightInPixels(height(boundingBox) / resolution)
	, resolution(resolution)
	, index(index)
	, dataStream(std::move(dataStream))
{
	if (widthInPixels * resolution != width(boundingBox) ||
	    heightInPixels * resolution != height(boundingBox))
		throw std::runtime_error("Block::Block(): bounding box doesn't match resolution");
}

Block::Block(const Line& line, int index)
	: Block(makeRectangle(line.getTileEastMin(), line.getTileNorthMin(), line.getTileEastMax(), line.getTileNorthMax()),
	             line.getResolution(), line.getTileDataSource(), index)
{}



Blocks::Blocks(const std::vector<Line>& lines)
{
	int lineNr = 0;
	for (const auto& line : lines)
	{
		auto block = Block(line, lineNr++);
		blocksTree.insert(std::make_pair(block.getBoundingBox(), block));

		resolutions.insert(line.getResolution());
	}

	if (!blocksTree.empty())
	{
		const auto bounds = blocksTree.bounds();
		boundingBox = makeRectangle(bounds.min_corner().get<0>(), bounds.min_corner().get<1>(),
			bounds.max_corner().get<0>(), bounds.max_corner().get<1>());
	}
	else
		boundingBox = makeRectangle(0, 0, 0, 0);
}

std::vector<Block> Blocks::getIntersectingBlocks(const Rectangle& box) const
{
	auto isNotOnlyBorderMatch = [&box](const TreeEntry& entry)
	{
		Rectangle intersectionBox;
		boost::geometry::intersection(entry.first, box, intersectionBox);

		auto intersectionArea = boost::geometry::area(intersectionBox);
		return intersectionArea != 0;
	};

	std::vector<TreeEntry> intersectingEntries;
	blocksTree.query(boost::geometry::index::intersects(box)
	                 && boost::geometry::index::satisfies(isNotOnlyBorderMatch),
	                 std::back_inserter(intersectingEntries));

	std::vector<Block> result;
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

}}
