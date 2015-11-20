#include "IndexBlocks.h"

#include <gmock/gmock.h>

using std::vector;

namespace {

IndexLine makeLine(int minX, int minY, int maxX, int maxY, int resolution = 1)
{
	return IndexLine(minX, maxX, minY, maxY, resolution, nullptr);
}

vector<int> getIndicesOfIntersectingBlocks(IndexBlocks& blocks, int minX, int minY, int maxX, int maxY)
{
	auto intersectingBlocks = blocks.getIntersectingBlocks(makeBox(minX, minY, maxX, maxY));

	vector<int> indices;
	indices.reserve(intersectingBlocks.size());
	for (auto& block : intersectingBlocks)
		indices.push_back(block.getIndex());

	return indices;
}

}

TEST(IndexBlock, constructors)
{
	const auto box = makeBox(-3, 2, 7, 6);
	IndexBlock block(box, 2, nullptr, 1);

	for (int i = 0; i < 2; ++i)
	{
		if (i == 1)
			block = IndexBlock(makeLine(-3, 2, 7, 6, 2), 1);

		EXPECT_EQ(box, block.getBoundingBox());
		EXPECT_EQ(10/2, block.getWidthInPixels());
		EXPECT_EQ(4/2, block.getHeightInPixels());
		EXPECT_EQ(2, block.getResolution());
		EXPECT_EQ(1, block.getIndex());
	}
}


TEST(IndexBlocks, constructorSetsBoundingBox)
{
	IndexBlocks blocks;
	EXPECT_EQ(makeBox(0, 0, 0, 0), blocks.getBoundingBox());

	blocks = IndexBlocks({ makeLine(-3, 2, 7, 6, 2), makeLine(2, -4, 8, 2, 1) });
	EXPECT_EQ(makeBox(-3, -4, 8, 6), blocks.getBoundingBox());
}

TEST(IndexBlocks, constructorSetsResolutions)
{
	vector<IndexLine> lines = {
		makeLine(0, 0, 10, 10, 10),
		makeLine(0, 0, 10, 10, 2),
		makeLine(0, 0, 10, 10, 5),
		makeLine(0, 0, 10, 10, 5),
	};
	IndexBlocks blocks(std::move(lines));

	EXPECT_THAT(blocks.getResolutions(), testing::ElementsAre(2, 5, 10));
}

TEST(IndexBlocks, insertsAndQueriesBlocksCorrectly)
{
	// overall: square from (0,0) to (2,2) with 3 differently sized blocks of identical resolution;
	// upper-right quadrant is overlapping
	vector<IndexLine> lines = {
		makeLine(0, 1, 2, 2),
		makeLine(0, 0, 1, 1),
		makeLine(1, 0, 2, 2)
	};
	IndexBlocks blocks(std::move(lines));

	auto all = getIndicesOfIntersectingBlocks(blocks, 0, 0, 2, 2);
	EXPECT_THAT(all, testing::ElementsAre(0, 1, 2));

	auto bottomLeftQuadrant = getIndicesOfIntersectingBlocks(blocks, 0, 0, 1, 1);
	EXPECT_THAT(bottomLeftQuadrant, testing::ElementsAre(1));

	auto bottomRightQuadrant = getIndicesOfIntersectingBlocks(blocks, 1, 0, 2, 1);
	EXPECT_THAT(bottomRightQuadrant, testing::ElementsAre(2));

	auto topLeftQuadrant = getIndicesOfIntersectingBlocks(blocks, 0, 1, 1, 2);
	EXPECT_THAT(topLeftQuadrant, testing::ElementsAre(0));

	auto topRightQuadrant = getIndicesOfIntersectingBlocks(blocks, 1, 1, 2, 2);
	EXPECT_THAT(topRightQuadrant, testing::ElementsAre(0, 2));

	auto centerPoint = getIndicesOfIntersectingBlocks(blocks, 1, 1, 1, 1);
	EXPECT_EQ(true, centerPoint.empty());

	auto adjacentRight = getIndicesOfIntersectingBlocks(blocks, 2, -666, 666, 666);
	EXPECT_EQ(true, adjacentRight.empty());
}

TEST(IndexBlocks, queriedBlocksAreOrderedByResolutionAndIndex)
{
	vector<IndexLine> lines = {
		makeLine(0, 0, 1, 1, 1),
		makeLine(0, 0, 2, 2, 2),
		makeLine(0, 0, 2, 2, 2)
	};
	IndexBlocks blocks(std::move(lines));

	auto indices = getIndicesOfIntersectingBlocks(blocks, 0, 0, 1, 1);
	EXPECT_THAT(indices, testing::ElementsAre(1, 2, 0));
}
