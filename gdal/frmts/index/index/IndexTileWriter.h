#pragma once

#include <iosfwd>

#include "IndexGeometry.h"

class IndexTileWriter
{
	std::ostream& destination;
	MapBox destinationArea;
	int resolution;

public:
	IndexTileWriter(std::ostream& destination, const MapBox& destinationArea, int resolution)
		: destination(destination)
		, destinationArea(destinationArea)
		, resolution(resolution)
	{}

	void write(std::istream& source, const MapBox& sourceArea);

private:
	void rewindDestination();
	void advance(std::istream& inputStream, int offset) const;
	void advance(std::ostream& outputStream, int offset) const;
	
	int getPixelToPoint(const MapBox& sourceArea, const MapPoint& firstCopiedPoint);

};