#include "IndexRasterBand.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include "IndexLine.h"
#include "IndexTileWriter.h"
#include "IndexConstants.h"

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace {
void writeTile(IndexTileWriter& writer, const IndexBlocks::MapTile& tileInformation)
{
	const auto& accessedTile = tileInformation.second;

	auto dataFile = accessedTile.getData();

	writer.write(*dataFile, tileInformation.first);
}
}

IndexRasterBand::IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks)
	: blocks(std::move(blocks))
{
	poDS = owningDataSet;
	nBand = 1;

	eDataType = GDT_Int16;

	nBlockXSize = this->blocks.getBlockXSize();
	nBlockYSize = this->blocks.getBlockYSize();

	undefValueLineLittleEndian.resize(nBlockXSize, ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES_LITTLE_ENDIAN);
	undefValueLineBigEndian.resize(nBlockXSize, ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES_BIG_ENDIAN);
}

CPLErr IndexRasterBand::IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage)
{
	typedef boost::iostreams::basic_array<char> Device;

	char* imageBuffer = static_cast<char*>(pImage);
	boost::iostreams::stream<Device> outputStream(imageBuffer, sizeof(std::int16_t) * blocks.getBlockXSize() * blocks.getBlockYSize());

	auto intersectingTiles = blocks.getIntersectingMapTiles(nBlockXOff, nBlockYOff);

	if(intersectingTiles.empty())
		fillBlockWithUndefLittleEndian(outputStream);
	else
	{
		auto requestedBlock = blocks.getBlockBox(nBlockXOff, nBlockYOff);

		IndexTileWriter writer(outputStream, requestedBlock, blocks.getPixelSquareSize());

		if(intersectingTiles.size() == 1 && boost::geometry::equals(intersectingTiles.front().first, requestedBlock))
		{
			auto dataFile = intersectingTiles.front().second.getData();
			outputStream << dataFile->rdbuf();
		}
		else
		{
			fillBlockWithUndefBigEndian(outputStream);

			boost::range::sort(intersectingTiles, 
							   [](const auto& tile1, const auto& tile2){ return tile1.second.getIndex() < tile2.second.getIndex(); });

			for (const auto& touchedTile : intersectingTiles)
				writeTile(writer, touchedTile);
		}

		convertToNativeByteOrder(pImage);
	}

	return CE_None;
}

void IndexRasterBand::convertToNativeByteOrder(void* pImage)
{
	std::int16_t* imageValues = static_cast<std::int16_t*>(pImage);
	const size_t writtenValues = blocks.getBlockXSize() * blocks.getBlockYSize();

	for (size_t i = 0; i < writtenValues; ++i)
		boost::endian::big_to_native_inplace(*(imageValues++));
}

GDALColorInterp IndexRasterBand::GetColorInterpretation()
{
	return GCI_GrayIndex;
}

void IndexRasterBand::fillBlockWithUndefLittleEndian(std::ostream& outputStream)
{
	fillBlock(outputStream, undefValueLineLittleEndian);
}

void IndexRasterBand::fillBlockWithUndefBigEndian(std::ostream& outputStream)
{
	fillBlock(outputStream, undefValueLineBigEndian);
}

void IndexRasterBand::fillBlock(std::ostream& outputStream, const std::vector<std::int16_t>& lineData)
{
	for (int i = 0; i < nBlockYSize; ++i)
		outputStream.write(reinterpret_cast<const char*>(lineData.data()), lineData.size()*sizeof(std::int16_t));
}

bool IndexRasterBand::tileCoversBlockCompletely(const MapBox& tileBox, const MapBox& blockBox)
{
	MapBox intersectionBox;

	boost::geometry::intersection(tileBox, blockBox, intersectionBox);

	return boost::geometry::equals(intersectionBox, blockBox);
}

