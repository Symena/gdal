#pragma once

#include "Blocks.h"
#include "DataOrientation.h"
#include "gdal.h"

namespace aircom_map {

class Renderer
{
public:
	using PixelType = std::int16_t;
	using UniqueDataPtr = std::unique_ptr<PixelType[]>;

private:
	const PixelType noDataValue = -9999;

	const Blocks& blocks;
	MapBox bounds; // in meters
	PixelType* data;
	DataOrientation dataOrientation;
	int widthInPixels;
	int heightInPixels;
	int resolution;
	GDALRIOResampleAlg downsamplingAlgorithm;
	GDALRIOResampleAlg upsamplingAlgorithm;
	Warnings& warnings;

public:
	Renderer(const Blocks& blocks, PixelType* data, DataOrientation dataOrientation, int widthInPixels,
		int heightInPixels, int resolution, MapPoint bottomLeftCornerInMeters,
		GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm,
		Warnings& warnings);

	const MapBox& getBoundingBox() const { return bounds; }

	void render();

	PixelType* getData() const { return data; }

	size_t getNumPixels() const { return static_cast<size_t>(widthInPixels) * heightInPixels; }

	void fillWithNoDataValue();

	// Reads a region (in meters) from a block, in its original resolution.
	// The region may be adjusted (expanded and/or re-aligned).
	// The data will be in native endianness and stored bottom-up.
	UniqueDataPtr readBlock(const Block& block, MapBox& region) const;

	// Resamples a block region to the target resolution.
	// The region may be adjusted.
	UniqueDataPtr resample(const PixelType* data, MapBox& region, int srcResolution) const;

	// Renders a region (in meters) in the target resolution into the result bitmap.
	void renderRegion(const PixelType* data, const MapBox& region);
};

}
