#include "IndexRasterBand.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include "IndexLine.h"
#include "IndexTileWriter.h"

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/endian/conversion.hpp>

IndexRasterBand::IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks)
	: blocks(std::move(blocks))
{
	poDS = owningDataSet;
	nBand = 1;

	eDataType = GDT_Int16;

	nBlockXSize = this->blocks.getBlockXSize();
	nBlockYSize = this->blocks.getBlockYSize();
}

CPLErr IndexRasterBand::IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage)
{
	memset(pImage, 0, blocks.getBlockXSize() * blocks.getBlockYSize() * sizeof(std::int16_t));

	auto intersectingTiles = blocks.getIntersectingMapTiles(nBlockXOff, nBlockYOff);

	if(intersectingTiles.empty())
		return CE_None;

	typedef boost::iostreams::basic_array<char> Device;

	char* imageBuffer = static_cast<char*>(pImage);
	boost::iostreams::stream<Device> outputStream(imageBuffer, sizeof(std::int16_t) * blocks.getBlockXSize() * blocks.getBlockYSize());

	IndexTileWriter writer(outputStream, blocks.getBlockBox(nBlockXOff, nBlockYOff), blocks.getPixelSquareSize());

	for(const auto& touchedTile : intersectingTiles)
	{
		const auto& accessedTile = touchedTile.second;

		auto dataFile = accessedTile.getData();

		writer.write(*dataFile, touchedTile.first);
	}

	//TODO: swap bytes
// 	std::int16_t* imageValues = static_cast<std::int16_t*>(pImage);
// 	const size_t writtenValues = blocks.getBlockXSize() * blocks.getBlockYSize();
// 
// 	for (size_t i = 0; i < writtenValues; ++i)
// 		boost::endian::big_to_native_inplace(*(imageValues++));

	return CE_None;
}

GDALColorInterp IndexRasterBand::GetColorInterpretation()
{
	return GCI_GrayIndex;
}

