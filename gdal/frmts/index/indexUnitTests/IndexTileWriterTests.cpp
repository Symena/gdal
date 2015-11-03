#include "IndexTileWriter.h"

#include <gtest/gtest.h>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/geometry.hpp>

using vec = std::vector<std::int16_t>;

class IndexTileWriterTests : public ::testing::Test
{
public:
	typedef boost::iostreams::basic_array<char> Device;
private:

	MapBox destination{{0,0},{2,2}};
	int pixelSize = 1;

	std::unique_ptr<IndexTileWriter> writer;

	vec destinationBuffer;
	std::int16_t initialDestinationValue = 0;

	std::unique_ptr<boost::iostreams::stream<Device>> memstream;

public:

	void setDestination(MapPoint minCorner, MapPoint maxCorner) { destination = MapBox(minCorner, maxCorner); }
	void setPixelSize(int pixelSize) { this->pixelSize = pixelSize; }
	void setInitialDestinationValue(std::int16_t initialDestinationValue) { this->initialDestinationValue = initialDestinationValue; }

	IndexTileWriter& getWriter()
	{
		if(!writer)
			initialize();

		return *writer;
	}

	const vec& getDestinationBuffer() { return destinationBuffer; }

	void write(MapBox source, const vec& data)
	{
		auto squarePixelArea = pixelSize * pixelSize;

		auto sourceArea = static_cast<int>(boost::geometry::area(source));
		if (sourceArea % squarePixelArea != 0)
			throw std::logic_error("source area has wrong size");

		auto requiredPixels = sourceArea / squarePixelArea;

		if(data.size() != requiredPixels)
			throw std::logic_error("wrong number of pixels");

		boost::iostreams::stream<Device> inputStream(const_cast<char*>(reinterpret_cast<const char*>(data.data())), data.size()*sizeof(std::int16_t));

		getWriter().write(inputStream, source);
	}

	bool destinationEqualTo(const std::vector<std::int16_t>& expectedDestination)
	{
		return destinationBuffer == expectedDestination;
	}

private:
	void initialize()
	{
		auto squarePixelArea = pixelSize * pixelSize;

		auto destinationArea = static_cast<int>(boost::geometry::area(destination));

		if(destinationArea % squarePixelArea != 0)
			throw std::logic_error("destination area has wrong size");

		destinationBuffer.resize(destinationArea / squarePixelArea, initialDestinationValue);

		memstream = std::make_unique<boost::iostreams::stream<Device>>(reinterpret_cast<char*>(destinationBuffer.data()), destinationBuffer.size()*sizeof(std::int16_t));

		writer = std::make_unique<IndexTileWriter>(*memstream, destination, pixelSize);
	}
};

TEST_F(IndexTileWriterTests, canWriteSingleFullSourceToDestination)
{
	write({{0,0},{2,2}},{1,2,3,4});

	EXPECT_EQ(vec({1,2,3,4}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_right)
{
	write({{1,0},{3,2}},{1,2,3,4});

	EXPECT_EQ(vec({0,1,0,3}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_down)
{
	write({{0,-1},{2,1}},{1,2,3,4});

	EXPECT_EQ(vec({0,0,1,2}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_right_down)
{
	write({{1,-1},{3,1}},{1,2,3,4});

	EXPECT_EQ(vec({0,0,0,1}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_left)
{
	write({{-1,0},{1,2}},{1,2,3,4});

	EXPECT_EQ(vec({2,0,4,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_left_down)
{
	write({{-1,-1},{1,1}},{1,2,3,4});

	EXPECT_EQ(vec({0,0,2,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_up)
{
	write({{0,1},{2,3}},{1,2,3,4});

	EXPECT_EQ(vec({3,4,0,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_left_up)
{
	write({{-1,1},{1,3}},{1,2,3,4});

	EXPECT_EQ(vec({4,0,0,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWritePartially_right_up)
{
	write({{1,1},{3,3}},{1,2,3,4});

	EXPECT_EQ(vec({0,3,0,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteSourceContainedInDestination)
{
	setDestination({0,0},{3,3});

	write({{1,1},{2,2}},{1});

	EXPECT_EQ(vec({0,0,0,0,1,0,0,0,0}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteSmallerSource_right)
{
	setDestination({0,0},{4,4});

	write({{3,1},{5,3}},{1,2,3,4});

	auto expected = vec({
		0,0,0,0,
		0,0,0,1,
		0,0,0,3,
		0,0,0,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteSmallerSource_left)
{
	setDestination({0,0},{4,4});

	write({{-1,1},{1,3}},{1,2,3,4});

	auto expected = vec({
		0,0,0,0,
		2,0,0,0,
		4,0,0,0,
		0,0,0,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteSmallerSource_up)
{
	setDestination({0,0},{4,4});

	write({{1,3},{3,5}},{1,2,3,4});

	auto expected = vec({
		0,3,4,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteSmallerSource_down)
{
	setDestination({0,0},{4,4});

	write({{1,-1},{3,1}},{1,2,3,4});

	auto expected = vec({
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,1,2,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteLargerSource_crossingHorizontally)
{
	setDestination({0,0},{3,3});

	write({{-1,1},{4,2}},{1,2,3,4,5});

	auto expected = vec({
		0,0,0,
		2,3,4,
		0,0,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteLargerSource_crossingVertically)
{
	setDestination({0,0},{3,3});

	write({{1,-1},{2,4}},{1,2,3,4,5});

	auto expected = vec({
		0,2,0,
		0,3,0,
		0,4,0});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteLargerSource_largerInEveryDimension)
{
	setDestination({0,0},{3,3});

	vec writeData;
	for (int i = 1; i <= 25; ++i)
		writeData.push_back(i);

	write({{-1,-1},{4,4}},writeData);

	auto expected = vec({
		7,8,9,
		12,13,14,
		17,18,19});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWrite_nonUnitResolution)
{
	setPixelSize(4);
	setDestination({-8,8}, {8,24});
	
	vec writeData;
	for (int i = 1; i <= 16; ++i)
		writeData.push_back(i);

	write({{0,0},{16,16}}, writeData);

	auto expected = vec({
		0,0,0,0,
		0,0,0,0,
		0,0,1,2,
		0,0,5,6});

	EXPECT_EQ(expected, getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteMultipleFullSourcesToDestination)
{
	write({{0,0},{2,2}}, {1,2,3,4});
	write({{0,0},{2,2}}, {5,6,7,8});

	EXPECT_EQ(vec({5,6,7,8}), getDestinationBuffer());
}

TEST_F(IndexTileWriterTests, canWriteMultiplePartialSourcesToDestination)
{
	write({{0,0},{1,1}}, {1});
	write({{1,0},{2,1}}, {2});
	write({{0,1},{1,2}}, {3});
	write({{1,1},{2,2}}, {4});

	EXPECT_EQ(vec({3,4,1,2}), getDestinationBuffer());
}

//empty space has -9999
//transparency and z order (multi full with some holes)
//block alignment
//endian-conversion
//use warning system