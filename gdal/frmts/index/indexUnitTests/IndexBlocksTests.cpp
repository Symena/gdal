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

TEST_F(IndexBlocksTests, accessingAnUnavailableBlockGivesEmptyOptional_noBlocksPresent)
{
	EXPECT_FALSE(hasBlock(0,0));
}

TEST_F(IndexBlocksTests, accessingAnUnavailableBlockGivesEmptyOptional_otherBlocksPresent)
{
	addLine(0, 1, 0, 1);

	EXPECT_FALSE(hasBlock(1,0));
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

TEST_F(IndexBlocksTests, recognizesHoleAsMissingTile_X)
{
	addLine(0, 1, 0, 1);
	addLine(2, 3, 0, 1);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}

TEST_F(IndexBlocksTests, recognizesHoleAsMissingTile_Y)
{
	addLine(0, 1, 0, 1);
	addLine(0, 1, 2, 3);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(0, 1));
	EXPECT_TRUE(hasBlock(0, 2));
}

TEST_F(IndexBlocksTests, holeRecognitionWorksWithNonUnitBlockSizes_X)
{
	setPixelSize(3);
	addLine(3, 9, 0, 3);
	addLine(15, 21, 0, 3);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(1, 0));
	EXPECT_TRUE(hasBlock(2, 0));
}

TEST_F(IndexBlocksTests, holeRecognitionWorksWithNonUnitBlockSizes_Y)
{
	setPixelSize(3);
	addLine(0, 3, 3, 9);
	addLine(0, 3, 15, 21);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_FALSE(hasBlock(0, 1));
	EXPECT_TRUE(hasBlock(0, 2));
}

//TODO: ragged tests

TEST_F(IndexBlocksTests, throwsIfNoLineHasMaximumSize_XGrows)
{
	addLine(0, 1, 0, 10);
	addLine(1, 11, 10, 11);

	EXPECT_THROW(getBlocks(), std::runtime_error);
}

TEST_F(IndexBlocksTests, throwsIfNoLineHasMaximumSize_YGrows)
{
	addLine(0, 10, 0, 1);
	addLine(10, 11, 0, 10);

	EXPECT_THROW(getBlocks(), std::runtime_error);
}

TEST_F(IndexBlocksTests, supportsPartialTileRightSide)
{
	setPixelSize(2);

	addLine(0, 4, 0, 2);
	addLine(4, 6, 0, 2);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(1, 0));

	auto block = getBlock(1, 0);

	EXPECT_EQ(0, block.getOffsetInBlockX());
	EXPECT_EQ(0, block.getOffsetInBlockY());
}

TEST_F(IndexBlocksTests, supportsPartialTileLeftSide)
{
	setPixelSize(2);

	addLine(0, 2, 0, 2);
	addLine(2, 6, 0, 2);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(1, 0));

	auto block = getBlock(0, 0);

	EXPECT_EQ(1, block.getOffsetInBlockX());
	EXPECT_EQ(0, block.getOffsetInBlockY());
}

TEST_F(IndexBlocksTests, supportsPartialTileUpperSide)
{
	setPixelSize(2);

	addLine(0, 2, 0, 4);
	addLine(0, 2, 4, 6);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));

	auto block = getBlock(0, 0);

	EXPECT_EQ(0, block.getOffsetInBlockX());
	EXPECT_EQ(1, block.getOffsetInBlockY());
}

TEST_F(IndexBlocksTests, supportsPartialTileLowerSide)
{
	setPixelSize(2);

	addLine(0, 2, 0, 2);
	addLine(0, 2, 2, 6);

	EXPECT_TRUE(hasBlock(0, 0));
	EXPECT_TRUE(hasBlock(0, 1));

	auto block = getBlock(0, 1);

	EXPECT_EQ(0, block.getOffsetInBlockX());
	EXPECT_EQ(0, block.getOffsetInBlockY());
}

//todo: partial tile in corners tests

TEST_F(IndexBlocksTests, reservesUndefValueVector)
{
	addLine(0, 10, 0, 20);

	auto undefLine = getBlocks().getUndefBlockLine();

	EXPECT_EQ(10, undefLine.size());
	EXPECT_EQ(-3624, undefLine.at(4));
}

TEST_F(IndexBlocksTests, undefValueVectorHasCorrectByteOrder)
{
	addLine(0, 10, 0, 20);

	auto undefLine = getBlocks().getUndefBlockLine();

	const std::uint8_t* data = reinterpret_cast<std::uint8_t*>(undefLine.data());

	EXPECT_EQ(0xD8, *data);
	EXPECT_EQ(0xF1, *(data + 1));
}