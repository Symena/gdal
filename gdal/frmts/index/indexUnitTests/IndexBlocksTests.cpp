#include <gmock/gmock.h>

#include "IndexBlocks.h"

class IndexBlocksTests : public testing::Test
{
	std::vector<IndexLine> lines;
	std::unique_ptr<IndexBlocks> blocks;
	IndexWarnings w;
	int pixelSize = 1;
public:

	IndexBlocks& getBlocks()
	{
		if(!blocks)
			blocks = std::make_unique<IndexBlocks>(lines);

		return *blocks;
	}

	void addLine(int eastMin, int eastMax, int northMin, int northMax, const std::string& filename = "file")
	{
		std::stringstream lineStream;
		lineStream << filename << " " << eastMin << " " << eastMax << " " << northMin << " " << northMax << " " << pixelSize << "\n";

		std::string s;
		std::getline(lineStream, s);

		lines.emplace_back(s, w);
	}

	void setPixelSize(int pixelSquareSize) { pixelSize = pixelSquareSize; }

	bool hasBlock(int xBlockOffset, int yBlockOffset)
	{
		return getBlocks().getBlock(xBlockOffset, yBlockOffset).is_initialized();
	}

	IndexBlock getBlock(int xBlockOffset, int yBlockOffset)
	{
		return getBlocks().getBlock(xBlockOffset, yBlockOffset).get();
	}
};

TEST_F(IndexBlocksTests, accessingAnUnavailableBlockGivesEmptyOptional)
{
	EXPECT_FALSE(hasBlock(0,0));
}

TEST_F(IndexBlocksTests, buildsSuccessfullyFromSingleLine)
{
	setPixelSize(3);
	addLine(-3, 6, -6, 9);

	auto block = getBlock(0, 0);

	EXPECT_EQ("file", block.getFile());
	EXPECT_EQ(3, block.getRasterSizeX());
	EXPECT_EQ(5, block.getRasterSizeY());
}

TEST_F(IndexBlocksTests, blocksWithLowerNorthingHaveHigherBlockIndex)
{
	addLine(0, 1, 0, 1, "low");
	addLine(0, 1, 1, 2, "high");

	EXPECT_EQ("high", getBlock(0,0).getFile());
	EXPECT_EQ("low", getBlock(0,1).getFile());
}

TEST_F(IndexBlocksTests, blocksWithLowerEastingHaveLowerBlockIndex)
{
	addLine(0, 1, 0, 1, "left");
	addLine(1, 2, 0, 1, "right");

	EXPECT_EQ("left", getBlock(0, 0).getFile());
	EXPECT_EQ("right", getBlock(1, 0).getFile());
}

TEST_F(IndexBlocksTests, recognizesHoleAsMissingTile)
{
	addLine(0, 1, 0, 1);
	addLine(2, 3, 0, 1);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}