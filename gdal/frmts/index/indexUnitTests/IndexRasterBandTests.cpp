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
			rasterBand = std::make_unique<IndexRasterBand>(owningDS, builder.create());

		return *rasterBand;
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
		.addTile().from(0, 0).to(2, 1)
		.addTile().from(2, 3).to(4, 4);

	auto band = getRasterBand();

	int xBlockSize, yBlockSize;
	band.GetBlockSize(&xBlockSize, &yBlockSize);

	EXPECT_EQ(2, xBlockSize);
	EXPECT_EQ(1, yBlockSize);
}

TEST_F(IndexRasterBandTests, writesSingleCompleteTile)
{
	getBlockBuilder()
		.addTile().from(0,0).to(1,1).withData({1});

	auto writtenData = getBlock(0, 0);

	EXPECT_EQ(vec{1}, writtenData);
}