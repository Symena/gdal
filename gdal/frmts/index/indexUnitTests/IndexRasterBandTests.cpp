#include <gmock/gmock.h>

#include "IndexRasterBand.h"
#include "IndexBlocksBuilder.h"

class IndexRasterBandTests : public testing::Test
{
	IndexDataset* owningDS = nullptr;
	IndexBlocksBuilder builder;
	
	std::unique_ptr<IndexRasterBand> rasterBand;

public:

	void setOwningDS(IndexDataset* owningDS) { this->owningDS = owningDS; }

	IndexRasterBand& getRasterBand()
	{
		if(!rasterBand)
			rasterBand = std::make_unique<IndexRasterBand>(owningDS, builder.create());

		return *rasterBand;
	}

	IndexBlocksBuilder& getBlockBuilder() { return builder; }
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
	this->getBlockBuilder()
		.addBlock(0, 2, 0, 1)
		.addBlock(2, 4, 3, 4);

	auto band = getRasterBand();

	int xBlockSize, yBlockSize;
	band.GetBlockSize(&xBlockSize, &yBlockSize);

	EXPECT_EQ(2, xBlockSize);
	EXPECT_EQ(1, yBlockSize);
}