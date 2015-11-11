#include <gmock/gmock.h>

#include "IndexBlocks.h"
#include "IndexBlocksBuilder.h"

class IndexBlocksTests : public testing::Test
{
	std::unique_ptr<IndexBlocks> blocks;
	int pixelSize = 1;
	IndexBlocksBuilder builder;
public:

	IndexBlocks& getBlocks()
	{
		if(!blocks)
			blocks = std::make_unique<IndexBlocks>(builder.create());

		return *blocks;
	}

	void addBlock(int eastMin, int eastMax, int northMin, int northMax)
	{
		builder.addTile().from(eastMin, northMin).to(eastMax, northMax);
	}

	void setPixelSize(int resolution) { builder.setPixelSize(resolution); }

	bool hasBlock(int xBlockOffset, int yBlockOffset)
	{
		return !getBlocks().getIntersectingMapTiles(xBlockOffset, yBlockOffset).empty();
	}

	IndexBlock getBlock(int xBlockOffset, int yBlockOffset)
	{
		auto blockEntry = getBlocks().getIntersectingMapTiles(xBlockOffset, yBlockOffset).at(0);

		return blockEntry.second;
	}
};

TEST_F(IndexBlocksTests, accessingAnUnavailableBlockGivesEmptyOptional_noBlocksPresent)
{
	EXPECT_FALSE(hasBlock(0,0));
}

TEST_F(IndexBlocksTests, accessingAnUnavailableBlockGivesEmptyOptional_otherBlocksPresent)
{
	addBlock(0, 1, 0, 1);

	EXPECT_FALSE(hasBlock(1,0));
}

TEST_F(IndexBlocksTests, buildsSuccessfullyFromSingleLine)
{
	setPixelSize(3);
	addBlock(-3, 6, -6, 9);

	auto block = getBlock(0, 0);

	EXPECT_EQ(3, block.getRasterSizeX());
	EXPECT_EQ(5, block.getRasterSizeY());
}

TEST_F(IndexBlocksTests, blocksWithLowerNorthingHaveHigherBlockIndex)
{
	addBlock(0, 1, 0, 1);
	addBlock(0, 1, 1, 2);

	EXPECT_EQ(1, getBlock(0,0).getIndex());
	EXPECT_EQ(0, getBlock(0,1).getIndex());
}

TEST_F(IndexBlocksTests, blocksWithLowerEastingHaveLowerBlockIndex)
{
	addBlock(1, 2, 0, 1);
	addBlock(0, 1, 0, 1);

	EXPECT_EQ(1, getBlock(0, 0).getIndex());
	EXPECT_EQ(0, getBlock(1, 0).getIndex());
}

TEST_F(IndexBlocksTests, recognizesHoleAsMissingTile_X)
{
	addBlock(0, 1, 0, 1);
	addBlock(2, 3, 0, 1);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}

TEST_F(IndexBlocksTests, recognizesHoleAsMissingTile_Y)
{
	addBlock(0, 1, 0, 1);
	addBlock(0, 1, 2, 3);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(0, 1));
	EXPECT_TRUE(hasBlock(0, 2));
}

TEST_F(IndexBlocksTests, holeRecognitionWorksWithNonUnitBlockSizes_X)
{
	setPixelSize(3);
	addBlock(3, 9, 0, 3);
	addBlock(15, 21, 0, 3);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}

TEST_F(IndexBlocksTests, holeRecognitionWorksWithNonUnitBlockSizes_Y)
{
	setPixelSize(3);
	addBlock(0, 3, 3, 9);
	addBlock(0, 3, 15, 21);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(0, 1));
	EXPECT_TRUE(hasBlock(0, 2));
}

TEST_F(IndexBlocksTests, supportsPartialTileUpperSide)
{
	setPixelSize(2);

	addBlock(0, 2, 0, 4);
	addBlock(0, 2, 4, 6);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));
}

TEST_F(IndexBlocksTests, supportsPartialTileLowerSide)
{
	setPixelSize(2);

	addBlock(0, 2, 0, 2);
	addBlock(0, 2, 2, 6);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));
}

TEST_F(IndexBlocksTests, supportsOverlappingBlocks)
{
	setPixelSize(50);

	addBlock(399975, 420025, 1139975, 1160025);
	addBlock(399975, 420025, 1159975, 1180025);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));
}

TEST_F(IndexBlocksTests, supportsOverlappingBlocks_east)
{
	setPixelSize(10);

	addBlock(0, 100, 0, 10);
	addBlock(90, 190, 0, 10);
	addBlock(180, 280, 0, 10);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}

TEST_F(IndexBlocksTests, supportsOverlappingBlocks_north)
{
	setPixelSize(10);

	addBlock(0, 10, 0, 100);
	addBlock(0, 10, 90, 190);
	addBlock(0, 10, 180, 280);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));
	EXPECT_TRUE(hasBlock(0, 2));
}