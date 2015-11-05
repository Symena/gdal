#include "IndexTileWriter.h"

#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/algorithms/equals.hpp>

#include "IndexConstants.h"

void IndexTileWriter::write(std::istream& source, const MapBox& sourceArea)
{
	source.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
	destination.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

	rewindDestination();

// 	if(boost::geometry::equals(sourceArea, destinationArea))
// 	{
// 		destination << source.rdbuf();
// 		return;
// 	}

	MapBox intersectingBox;
	boost::geometry::intersection(destinationArea, sourceArea, intersectingBox);

	const auto firstCopiedPoint = upper_left(intersectingBox);

	const auto pixelInitialSkipSource = getPixelToPoint(sourceArea, firstCopiedPoint);
	const auto pixelInitialSkipDestination = getPixelToPoint(destinationArea, firstCopiedPoint);

	advance(source, pixelInitialSkipSource * sizeof(std::int16_t));
	advance(destination, pixelInitialSkipDestination * sizeof(std::int16_t));

	assert(static_cast<int>(sizeof(std::int16_t)) * (width(sourceArea) - width(intersectingBox)) % pixelSquareSize == 0);
	assert(static_cast<int>(sizeof(std::int16_t)) * (width(destinationArea) - width(intersectingBox)) % pixelSquareSize == 0);

	const auto byteSkipSource = static_cast<int>(sizeof(std::int16_t)) * (width(sourceArea) - width(intersectingBox)) / pixelSquareSize;
	const auto byteSkipDestination = static_cast<int>(sizeof(std::int16_t)) * (width(destinationArea) - width(intersectingBox)) / pixelSquareSize;

	assert(height(intersectingBox) % pixelSquareSize == 0);
	assert(width(intersectingBox) % pixelSquareSize == 0);

	const auto linesToWrite = height(intersectingBox) / pixelSquareSize;
	int valuesToReadPerLine = width(intersectingBox) / pixelSquareSize;

	for (int line = 0; line < linesToWrite; ++line)
	{
		for (int i = 0; i < valuesToReadPerLine; ++i)
		{
			std::int16_t value;
			source.read(reinterpret_cast<char*>(&value), sizeof(std::int16_t));

			if(value != ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES_BIG_ENDIAN)
				destination.write(reinterpret_cast<char*>(&value), sizeof(std::int16_t));
			else
				advance(destination, sizeof(std::int16_t));
		}

		if(line < linesToWrite - 1)
		{
			advance(source, byteSkipSource);
			advance(destination, byteSkipDestination);
		}
	}
}

void IndexTileWriter::rewindDestination()
{
	destination.seekp(0, std::ios::beg);
}

void IndexTileWriter::advance(std::istream& inputStream, int offset) const
{
	inputStream.seekg(offset, std::ios::cur);
}

void IndexTileWriter::advance(std::ostream& outputStream, int offset) const
{
	outputStream.seekp(offset, std::ios::cur);
}

int IndexTileWriter::getPixelToPoint(const MapBox& sourceArea, const MapPoint& firstCopiedPoint)
{
	const auto fullLinesToSkip = (sourceArea.max_corner().get<1>() - firstCopiedPoint.get<1>()) / pixelSquareSize;
	const auto fullLineSkipMeters = width(sourceArea) * fullLinesToSkip;
	const auto inLineSkipMeters = firstCopiedPoint.get<0>() - sourceArea.min_corner().get<0>();

	assert(fullLineSkipMeters >= 0);
	assert(inLineSkipMeters >= 0);

	const auto metersToSkip = fullLineSkipMeters + inLineSkipMeters;

	if(metersToSkip % pixelSquareSize != 0)
		throw std::runtime_error("Box offset is not a multiple of the pixel size");

	return metersToSkip / pixelSquareSize;
}
