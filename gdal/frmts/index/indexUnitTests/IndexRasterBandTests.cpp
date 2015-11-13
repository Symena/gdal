#include <gmock/gmock.h>

#include "IndexRasterBand.h"
#include "IndexBlocksBuilder.h"

class IndexRasterBandTests : public ::testing::Test
{
	IndexDataset* owningDS = nullptr;
	IndexBlocksBuilder builder;
	
	std::unique_ptr<IndexRasterBand> rasterBand;

public:

	using vec = std::vector<std::int16_t>;

	void setOwningDS(IndexDataset* owningDS) { this->owningDS = owningDS; }

	IndexRasterBand& getRasterBand()
	{
		if(!rasterBand)
			rasterBand = std::make_unique<IndexRasterBand>(owningDS);

		return *rasterBand;
	}

	int getBlockSizeX()
	{
		int xBlockSize, yBlockSize;
		getRasterBand().GetBlockSize(&xBlockSize, &yBlockSize);

		return xBlockSize;
	}

	int getBlockSizeY()
	{
		int xBlockSize, yBlockSize;
		getRasterBand().GetBlockSize(&xBlockSize, &yBlockSize);

		return yBlockSize;
	}

	IndexBlocksBuilder& getBlockBuilder() { return builder; }

	vec getBlock(int blockXOffset, int blockYOffset)
	{
		auto& band = getRasterBand();

		int xBlockSize, yBlockSize;
		band.GetBlockSize(&xBlockSize, &yBlockSize);

		static const std::int16_t valueRepresentingUninitializedMemory = 3402;

		vec result(xBlockSize*yBlockSize, valueRepresentingUninitializedMemory);

		band.IReadBlock(blockXOffset, blockYOffset, reinterpret_cast<void*>(result.data()));

		return result;
	}
};

TEST_F(IndexRasterBandTests, setsOwningDataset)
{
	setOwningDS(reinterpret_cast<IndexDataset*>(4));

	auto band = getRasterBand();

	EXPECT_EQ(4, reinterpret_cast<size_t>(band.GetDataset()));
}

TEST_F(IndexRasterBandTests, isAlwaysTheFirstBand)
{
	auto band = getRasterBand();

	EXPECT_EQ(1, band.GetBand());
}

TEST_F(IndexRasterBandTests, hasInt16DataFormat)
{
	auto band = getRasterBand();

	EXPECT_EQ(GDT_Int16, band.GetRasterDataType());
}

TEST_F(IndexRasterBandTests, setsBlockSize)
{
	getBlockBuilder()
		.addBlock().from(0, 0).to(2, 1)
		.addBlock().from(2, 3).to(4, 4);

	auto band = getRasterBand();

	int xBlockSize, yBlockSize;
	band.GetBlockSize(&xBlockSize, &yBlockSize);

	EXPECT_EQ(2, xBlockSize);
	EXPECT_EQ(1, yBlockSize);
}

TEST_F(IndexRasterBandTests, writesSingleCompleteTile)
{
	getBlockBuilder()
		.addBlock().from(0,0).to(1,1).withData({1});

	auto writtenData = getBlock(0, 0);

	EXPECT_EQ(vec{1}, writtenData);
}

TEST_F(IndexRasterBandTests, blocksWhichDontExistAreFilledWithDoesNotExistValue)
{
	getBlockBuilder()
		.addBlock().from(0, 0).to(2, 2)
		.addBlock().from(6, 0).to(8, 2);

	auto writtenData = getBlock(1,0);

	EXPECT_EQ(vec(4, -9999), writtenData);
}

TEST_F(IndexRasterBandTests, blocksWhichAreOnlyPartiallyDefinedHaveUndefinedValues)
{
	getBlockBuilder()
		.addBlock().from(0, 0).to(1, 1).withData({1})
		.addBlock().from(2, 0).to(4, 2);

	ASSERT_EQ(2, getBlockSizeX());
	ASSERT_EQ(2, getBlockSizeY());

	auto writtenData = getBlock(0, 0);

	EXPECT_EQ(vec({-9999,-9999,1,-9999}), writtenData);
}

TEST_F(IndexRasterBandTests, supportsTileTransparency)
{
	getBlockBuilder()
		.addBlock().from(0, 0).to(2, 2).withData({-9999, -9999, 1, 2})
		.addBlock().from(0, 0).to(2, 2).withData({3, 4, -9999, -9999});

	auto expectedData = vec{3,4,1,2};

	EXPECT_EQ(expectedData, getBlock(0,0));
}

TEST_F(IndexRasterBandTests, writesDataWithCorrectZOrder)
{
	getBlockBuilder()
		.addBlock().from(0, 1).to(2, 3).withData({3,3,3,3}) //li ob
		.addBlock().from(1, 1).to(3, 3).withData({4,4,4,4}) //re ob
		.addBlock().from(0, 0).to(2, 2).withData({1,1,1,1}) //li u
		.addBlock().from(1, 0).to(3, 2).withData({2,2,2,2}) //re u
		.addBlock().from(2, 1).to(4, 3)
	;

	auto expectedData = vec{3,4,1,2};

	EXPECT_EQ(expectedData, getBlock(0, 0));
}

TEST_F(IndexRasterBandTests, alignsBlocksToReferenceBlock_X)
{
	getBlockBuilder()
		.addBlock().from(0,0).to(1,1).withData({1})
		.addBlock().from(1,0).to(3,1);

	ASSERT_EQ(2, getBlockSizeX());
	ASSERT_EQ(1, getBlockSizeY());

	auto expectedData = vec{-9999, 1};

	EXPECT_EQ(expectedData, getBlock(0,0));
}

TEST_F(IndexRasterBandTests, alignsBlocksToReferenceBlock_Y)
{
	getBlockBuilder()
		.addBlock().from(0, 0).to(1, 1).withData({1})
		.addBlock().from(0, -2).to(1, 0);

	ASSERT_EQ(1, getBlockSizeX());
	ASSERT_EQ(2, getBlockSizeY());

	auto expectedData = vec{-9999, 1};

	EXPECT_EQ(expectedData, getBlock(0, 0));
}

TEST_F(IndexRasterBandTests, fillsValuesWithUnknownInPresenceOfExceptions)
{
	getBlockBuilder()
		.addBlock().from(0,0).to(1,1).withData({}); //not enough data causes an exception

	auto expectedData = vec{-9999};

	EXPECT_EQ(expectedData, getBlock(0,0));
}