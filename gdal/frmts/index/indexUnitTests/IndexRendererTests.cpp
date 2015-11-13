#include "IndexRenderer.h"
#include "IndexBlocksBuilder.h"

#include <gmock/gmock.h>

using std::int16_t;
using std::vector;

struct IndexRendererTest : public testing::Test
{
	IndexBlocks blocks;

protected:
	IndexBlocksBuilder builder;
	IndexWarnings warnings;

	IndexRenderer createRenderer(const MapBox& region, int resolution = 1,
		GDALRIOResampleAlg downsamplingAlgorithm = GDALRIOResampleAlg::GRIORA_NearestNeighbour,
		GDALRIOResampleAlg upsamplingAlgorithm = GDALRIOResampleAlg::GRIORA_NearestNeighbour)
	{
		blocks = builder.create();
		return IndexRenderer(blocks, region, resolution, downsamplingAlgorithm, upsamplingAlgorithm, warnings);
	}

	static IndexBlock makeBlock(int minX, int minY, int maxX, int maxY, int resolution, vector<int16_t> topDownData)
	{
		for (auto& pixel : topDownData)
			boost::endian::native_to_big_inplace(pixel);

		const size_t numDataPixels = topDownData.size();

		IndexBlock block(makeBox(minX, minY, maxX, maxY), resolution,
			std::make_shared<VectorBackedStreamSource>(std::move(topDownData)), 0);

		assert(numDataPixels == static_cast<size_t>(block.getWidthInPixels()) * block.getHeightInPixels());

		return block;
	}

	static void checkPixels(IndexRenderer& renderer, const vector<int16_t>& expected)
	{
		const size_t numActualPixels = renderer.getNumPixels();
		ASSERT_EQ(expected.size(), numActualPixels);

		checkPixels(renderer.getResult(), expected);
	}

	static void checkPixels(const IndexRenderer::UniqueDataPtr& actual, const vector<int16_t>& expected)
	{
		for (size_t i = 0; i < expected.size(); ++i)
			EXPECT_EQ(expected[i], actual[i]);
	}
};


// fillWithNoDataValue()

TEST_F(IndexRendererTest, fillWithNoDataValue)
{
	auto renderer = createRenderer(makeBox(0, 0, 4, 2), 2); // 2x1 pixels
	EXPECT_EQ(2, renderer.getWidthInPixels());
	EXPECT_EQ(1, renderer.getHeightInPixels());
	EXPECT_EQ(2, renderer.getNumPixels());

	renderer.fillWithNoDataValue();

	checkPixels(renderer, { -9999, -9999 });
}


// readBlock()

TEST_F(IndexRendererTest, readBlock_full)
{
	auto renderer = createRenderer(MapBox());

	auto block = makeBlock(0, 0, 4, 4, 2, { 0, 1, 2, 3 });
	auto region = block.getBoundingBox();

	auto actual = renderer.readBlock(block, region);

	ASSERT_TRUE(region == block.getBoundingBox()); // unchanged
	checkPixels(actual, { 2, 3, 0, 1 });           // rows are flipped (bottom-up)
}

TEST_F(IndexRendererTest, readBlock_partial)
{
	auto renderer = createRenderer(MapBox());

	auto block = makeBlock(0, 0, 8, 8, 2,
		{  0,  1,  2,  3,
		   4,  5,  6,  7,
		   8,  9, 10, 11,
		  12, 13, 14, 15 });
	auto region = makeBox(4, 1, 7, 6);

	auto actual = renderer.readBlock(block, region);

	ASSERT_TRUE(region == makeBox(4, 0, 8, 6)); // expanded
	checkPixels(actual,
		{ 14, 15,
		  10, 11,
		   6,  7 });
}

TEST_F(IndexRendererTest, readBlock_returnsNullForNoBlockData)
{
	auto renderer = createRenderer(MapBox());

	IndexBlock block(makeBox(0, 0, 1, 1), 1, nullptr, 0);
	auto region = block.getBoundingBox();

	auto actual = renderer.readBlock(block, region);

	EXPECT_EQ(nullptr, actual);
	EXPECT_TRUE(region == block.getBoundingBox()); // unchanged
}


// renderRegion()

TEST_F(IndexRendererTest, renderRegion_doesNotCopyNoDataPixels)
{
	auto renderer = createRenderer(makeBox(0, 0, 6, 2), 2); // 3x1 pixels

	int16_t src1[] = { 0, -9999,     1 };
	int16_t src2[] = { 2, -9999, -9999 };

	renderer.fillWithNoDataValue();
	renderer.renderRegion(src1, renderer.getBoundingBox());
	renderer.renderRegion(src2, renderer.getBoundingBox());

	checkPixels(renderer, { 2, -9999, 1 });
}

TEST_F(IndexRendererTest, renderRegion_realignsSourceCorrectly)
{
	auto renderer = createRenderer(makeBox(0, 0, 12, 8), 4); // 3x2 pixels

	int16_t src1[] = { 0, 1, 2 };
	auto region1 = makeBox(-2, -1, 10, 3); // offset: (-0.5, -0.25) pixels

	int16_t src2[] = { 3, 4, 5 };
	auto region2 = makeBox(1, 2, 13, 6);   // offset: (0.25, 0.5) pixels

	renderer.fillWithNoDataValue();
	renderer.renderRegion(src1, region1);
	renderer.renderRegion(src2, region2);

	checkPixels(renderer,
		{ 0, 1, 2,
		  3, 4, 5 });
}

TEST_F(IndexRendererTest, renderRegion_sourceIsClipped)
{
	auto renderer = createRenderer(makeBox(0, 0, 12, 8), 4); // 3x2 pixels

	int16_t src1[] = { 0, 1, 2, 3, 4, 5 };
	auto region1 = makeBox(-7, 0, 17, 4);    // offset: (-1.75, 0) pixels

	int16_t src2[] = { 6, 7, 8, 9, 10, 11 }; // vertical
	auto region2 = makeBox(8, -13, 12, 11);  // offset: (2, -3.25) pixels

	int16_t src3[] =
		{ 666, 667,
		  668, 669 };
	auto region3 = makeBox(1000, -1000, 1008, -992); // completely outside

	renderer.fillWithNoDataValue();
	renderer.renderRegion(src1, region1);
	renderer.renderRegion(src2, region2);
	renderer.renderRegion(src3, region3);

	checkPixels(renderer,
		{     2,     3,  9,
		  -9999, -9999, 10 });
}


// render()

TEST_F(IndexRendererTest, render_noResampling)
{
	// resampling needs GDAL redistributable :/

	const auto region = makeBox(0, 0, 8, 6); // 4x3 pixels

	builder.addBlock().from(2, 0).to(8, 4).resolution(2).withData(
		{ 0, 1, 2, // top-down!
		  3, 4, 5 });
	builder.addBlock().from(0, 0).to(4, 4).resolution(2).withData(
		{ 10,    11,
		  12, -9999 });

	auto renderer = createRenderer(region, 2);

	renderer.render();

	checkPixels(renderer,
		{    12,     3,     4,     5, // bottom-up
		     10,    11,     1,     2,
		  -9999, -9999, -9999, -9999 });
}
