#pragma once

#include "IndexBlocks.h"
#include "gdal.h"

class IndexRenderer
{
public:
	using PixelType = std::int16_t;
	using UniqueDataPtr = std::unique_ptr<PixelType[]>;

private:
	const PixelType noDataValue = -9999;

	const IndexBlocks& blocks;
	MapBox bounds; // in meters
	int widthInPixels;
	int heightInPixels;
	int resolution;
	GDALRIOResampleAlg downsamplingAlgorithm;
	GDALRIOResampleAlg upsamplingAlgorithm;
	IndexWarnings& warnings;

	UniqueDataPtr data; // bottom-up

public:
	IndexRenderer(const IndexBlocks& blocks, const MapBox& region, int resolution,
		GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm,
		IndexWarnings& warnings);
	IndexRenderer(const IndexBlocks& blocks, UniqueDataPtr data, int widthInPixels,
		int heightInPixels, int resolution, MapPoint bottomLeftCornerInMeters,
		GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm,
		IndexWarnings& warnings);

	const MapBox& getBoundingBox() const { return bounds; }

	void render();

	// Returns the bottom-up bitmap.
	UniqueDataPtr getResult() { return std::move(data); }
	int getWidthInPixels() const { return widthInPixels; }
	int getHeightInPixels() const { return heightInPixels; }
	size_t getNumPixels() const { return static_cast<size_t>(widthInPixels) * heightInPixels; }

	void fillWithNoDataValue();

	// Reads a region (in meters) from a block, in its original resolution.
	// The region may be adjusted (expanded and/or re-aligned).
	// The data will be in native endianness and stored bottom-up.
	UniqueDataPtr readBlock(const IndexBlock& block, MapBox& region) const;

	// Resamples a block region to the target resolution.
	// The region may be adjusted.
	UniqueDataPtr resample(const PixelType* data, MapBox& region, int srcResolution) const;

	// Renders a region (in meters) in the target resolution into the result bitmap.
	void renderRegion(const PixelType* data, const MapBox& region);
};
