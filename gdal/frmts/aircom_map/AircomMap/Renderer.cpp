#include "Renderer.h"
#include "Resampling.h"

#include <boost/endian/conversion.hpp>
#include <boost/geometry/algorithms/intersection.hpp>

namespace aircom_map {

Renderer::Renderer(const Blocks& blocks, PixelType* data, DataOrientation dataOrientation, int widthInPixels,
	int heightInPixels, int resolution, MapPoint bottomLeftCornerInMeters,
	GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm,
	Warnings& warnings)
	: blocks(blocks)
	, bounds(bottomLeftCornerInMeters, bottomLeftCornerInMeters + MapPoint(widthInPixels * resolution, heightInPixels * resolution))
	, data(data)
	, dataOrientation(dataOrientation)
	, widthInPixels(widthInPixels)
	, heightInPixels(heightInPixels)
	, resolution(resolution)
	, downsamplingAlgorithm(downsamplingAlgorithm)
	, upsamplingAlgorithm(upsamplingAlgorithm)
	, warnings(warnings)
{}


void Renderer::render()
{
	fillWithNoDataValue();

	for (auto& block : blocks.getIntersectingBlocks(bounds))
	{
		// TODO: skip unnecessarily fine blocks?

		MapBox srcRegion;
		auto& blockBounds = block.getBoundingBox();
		boost::geometry::intersection(bounds, blockBounds, srcRegion);

		try
		{
			auto srcData = readBlock(block, srcRegion);

			if (block.getResolution() != resolution)
				srcData = resample(srcData.get(), srcRegion, block.getResolution());

			renderRegion(srcData.get(), srcRegion);
		}
		catch (const std::exception& e)
		{
			warnings.add("Rendering block from (%d, %d) to (%d, %d) (index %d) failed: '%s', skipping",
				blockBounds.min_corner().get<0>(), blockBounds.min_corner().get<1>(),
				blockBounds.max_corner().get<0>(), blockBounds.max_corner().get<1>(),
				block.getIndex(), e.what());
		}
	}
}

void Renderer::fillWithNoDataValue()
{
	const size_t numPixels = getNumPixels();

	/*
	const size_t numPixelsPerSizeT = sizeof(size_t) / sizeof(PixelType);

	size_t noDataValueSizeT;
	for (size_t i = 0; i < numPixelsPerSizeT; ++i)
		reinterpret_cast<PixelType*>(&noDataValueSizeT)[i] = noDataValue;

	PixelType* data = this->data.get();

	size_t* const dataSizeT = reinterpret_cast<size_t*>(data);
	const size_t numSizeTs = numPixels / numPixelsPerSizeT;
	for (size_t i = 0; i < numSizeTs; ++i)
		dataSizeT[i] = noDataValueSizeT;

	for (size_t i = numSizeTs * numPixelsPerSizeT; i < numPixels; ++i)
		data[i] = noDataValue;
	*/

	for (size_t i = 0; i < numPixels; ++i)
		data[i] = noDataValue;
}

Renderer::UniqueDataPtr Renderer::readBlock(const Block& block, MapBox& region) const
{
	auto stream = block.getData(warnings);
	if (!stream)
	{
		region = makeBox(0, 0, 0, 0);
		return nullptr;
	}

	const auto& blockBounds = block.getBoundingBox();
	const MapPoint blockTopLeft = upper_left(blockBounds);
	const MapPoint regionTopLeft = upper_left(region);

	// in meters:
	const int leftOffset = regionTopLeft.get<0>() - blockTopLeft.get<0>();
	const int topOffset = blockTopLeft.get<1>() - regionTopLeft.get<1>(); // blocks store rows top-down
	const int rightOffset = leftOffset + width(region);
	const int bottomOffset = topOffset + height(region);
	assert(leftOffset >= 0 && rightOffset <= width(blockBounds)
		&& topOffset >= 0 && bottomOffset <= height(blockBounds));

	// now compute the region of pixels to be read
	const int blockRes = block.getResolution();
	const int leftPixelOffset = leftOffset / blockRes;                    // floor
	const int topPixelOffset = topOffset / blockRes;                      // floor
	const int rightPixelOffset = (rightOffset + blockRes-1) / blockRes;   // ceiling
	const int bottomPixelOffset = (bottomOffset + blockRes-1) / blockRes; // ceiling

	const int regionWidthInPixels = rightPixelOffset - leftPixelOffset;
	const int regionHeightInPixels = bottomPixelOffset - topPixelOffset;

	const size_t numRegionPixels = static_cast<size_t>(regionWidthInPixels) * regionHeightInPixels;
	auto result = std::make_unique<PixelType[]>(numRegionPixels);

	// read the raw data, flipping the row order (first result row is bottom row)
	stream->seekg((static_cast<size_t>(topPixelOffset) * block.getWidthInPixels() + leftPixelOffset) * sizeof(PixelType));
	const size_t rowSkip = (block.getWidthInPixels() - regionWidthInPixels) * sizeof(PixelType);
	for (int y = 0; y < regionHeightInPixels; ++y)
	{
		const size_t flippedY = regionHeightInPixels-1 - y;
		stream->read(reinterpret_cast<char*>(result.get() + flippedY * regionWidthInPixels),
			regionWidthInPixels * sizeof(PixelType));

		if (flippedY != 0)
			stream->seekg(rowSkip, std::ios_base::cur);
	}

	// convert pixels: big-endian => native endianness
	for (size_t i = 0; i < numRegionPixels; ++i)
		boost::endian::big_to_native_inplace(result[i]);

	// inform the caller about the actually covered region (in meters)
	region.min_corner() = blockBounds.min_corner() +
		MapPoint(leftPixelOffset * blockRes, (block.getHeightInPixels() - bottomPixelOffset) * blockRes);
	region.max_corner() = blockBounds.min_corner() +
		MapPoint(rightPixelOffset * blockRes, (block.getHeightInPixels() - topPixelOffset) * blockRes);

	return result;
}

Renderer::UniqueDataPtr Renderer::resample(const PixelType* data, MapBox& region, int srcResolution) const
{
	const int srcWidth = width(region) / srcResolution;
	const int srcHeight = height(region) / srcResolution;

	const double scalingFactor = static_cast<double>(srcResolution) / resolution;
	const int newWidth = static_cast<int>(std::ceil(srcWidth * scalingFactor));
	const int newHeight = static_cast<int>(std::ceil(srcHeight * scalingFactor));

	const size_t numNewPixels = static_cast<size_t>(newWidth) * newHeight;
	if (numNewPixels == 0)
	{
		region = makeBox(0, 0, 0, 0);
		return nullptr;
	}

	auto newData = std::make_unique<PixelType[]>(numNewPixels);

	const auto algorithm = (scalingFactor < 1 ? downsamplingAlgorithm : upsamplingAlgorithm);
	aircom_map::resample(data, srcWidth, srcHeight, newData.get(), newWidth, newHeight, GDALDataType::GDT_Int16, algorithm, noDataValue);

	region.max_corner() = region.min_corner() + MapPoint(newWidth * resolution, newHeight * resolution);

	return newData;
}

void Renderer::renderRegion(const PixelType* data, const MapBox& region)
{
	const int res = this->resolution;

	const int srcWidthInPixels = width(region) / res;
	const int srcHeightInPixels = height(region) / res;
	assert(srcWidthInPixels * res == width(region));
	assert(srcHeightInPixels * res == height(region));

	// in meters:
	const auto bottomLeftOffset = region.min_corner() - this->bounds.min_corner();

	auto round = [](int numerator, int denominator)
	{
		double exact = static_cast<double>(numerator) / denominator;
		return static_cast<int>(std::floor(exact + 0.5)); // round halfway cases to the right
	};

	// compute the offset of the source in the global bitmap, in pixels:
	const int leftPixelOffset = round(bottomLeftOffset.get<0>(), res);
	const int bottomPixelOffset = round(bottomLeftOffset.get<1>(), res);

	// clip the source region, in pixels, to be rendered into the global bitmap
	const int srcPixelMinX = (leftPixelOffset < 0 ? -leftPixelOffset : 0);
	const int srcPixelMinY = (bottomPixelOffset < 0 ? -bottomPixelOffset : 0);
	const int srcPixelMaxX = std::min(srcWidthInPixels, this->widthInPixels - leftPixelOffset);
	const int srcPixelMaxY = std::min(srcHeightInPixels, this->heightInPixels - bottomPixelOffset);

	// render
	for (int y = srcPixelMinY; y < srcPixelMaxY; ++y)
	{
		size_t dstY = bottomPixelOffset + y;
		if (dataOrientation == DataOrientation::TopDown)
			dstY = this->heightInPixels - 1 - dstY;

		PixelType* const dstRow = this->data + dstY * this->widthInPixels + leftPixelOffset;
		const PixelType* const srcRow = data + static_cast<size_t>(y) * srcWidthInPixels;

		for (int x = srcPixelMinX; x < srcPixelMaxX; ++x)
		{
			const PixelType pixel = srcRow[x];
			if (pixel != noDataValue)
				dstRow[x] = pixel;
		}
	}
}

}
