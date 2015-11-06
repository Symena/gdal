#include "IndexRasterBand.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include "IndexLine.h"
#include "IndexTileWriter.h"
#include "IndexConstants.h"
#include "IndexWarnings.h"
#include "IndexWarningsReporter.h"

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/endian/conversion.hpp>

namespace {
void writeTile(IndexTileWriter& writer, const IndexBlocks::MapTile& tileInformation, IndexWarnings& warnings)
{
	const auto& accessedTile = tileInformation.second;

	auto dataFile = accessedTile.getData(warnings);

	if(dataFile)
		writer.write(*dataFile, tileInformation.first);
}
}

IndexRasterBand::IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks, boost::optional<IndexClutterCodes> codes)
	: blocks(std::move(blocks))
	, clutterCodes(std::move(codes))
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
	boost::iostreams::stream<Device> outputStream(static_cast<char*>(pImage), sizeof(std::int16_t) * blocks.getBlockXSize() * blocks.getBlockYSize());

	auto outputData = static_cast<std::int16_t*>(pImage);

	IReadBock(nBlockXOff, nBlockYOff, outputStream, outputData);
	
	return CE_None;
}

void IndexRasterBand::IReadBock(int nBlockXOff, int nBlockYOff, std::ostream& outputStream, std::int16_t* outputData)
{
	IndexWarnings warnings;
	IndexWarningsReporter warningsReporter(warnings);

	IndexWarningsContext blockContext(warnings, "Reading block (%1%,%2%): ", nBlockXOff, nBlockYOff);

	try
	{
		readTilesIntoBlock(nBlockXOff, nBlockYOff, outputStream, outputData, warnings);
	}
	catch (const std::exception& e)
	{
		fillBlockWithUndefLittleEndianForException(outputStream);

		warnings.add("%3%", nBlockXOff, nBlockYOff, e.what());
	}
	catch(...)
	{
		fillBlockWithUndefLittleEndianForException(outputStream);

		warnings.add("Unknown exception", nBlockXOff, nBlockYOff);
	}
}

void IndexRasterBand::readTilesIntoBlock(int nBlockXOff, int nBlockYOff, std::ostream& outputStream, std::int16_t* outputData, IndexWarnings& warnings)
{
	auto intersectingTiles = blocks.getIntersectingMapTiles(nBlockXOff, nBlockYOff);

	if (intersectingTiles.empty())
		fillBlockWithUndefLittleEndian(outputStream);
	else
	{
		readIntersectingTilesIntoBlock(outputStream, intersectingTiles, blocks.getBlockBox(nBlockXOff, nBlockYOff), warnings);

		convertToNativeByteOrder(outputData);
	}
}

bool IndexRasterBand::singleTileMatchesBlockPerfectly(const std::vector<IndexBlocks::MapTile>& intersectingTiles, const MapBox& requestedBlock)
{
	return intersectingTiles.size() == 1 && boost::geometry::equals(intersectingTiles.front().first, requestedBlock);
}

void IndexRasterBand::convertToNativeByteOrder(std::int16_t* outputData)
{
	const size_t writtenValues = blocks.getBlockXSize() * blocks.getBlockYSize();

	for (size_t i = 0; i < writtenValues; ++i)
		boost::endian::big_to_native_inplace(*(outputData++));
}

char** IndexRasterBand::GetCategoryNames()
{
	return clutterCodes->getClutterCodes();
}

void IndexRasterBand::readIntersectingTilesIntoBlock(std::ostream& outputStream, const std::vector<IndexBlocks::MapTile>& intersectingTiles, const MapBox& requestedBlock, IndexWarnings& warnings)
{
	IndexTileWriter writer(outputStream, requestedBlock, blocks.getPixelSquareSize());

	if (singleTileMatchesBlockPerfectly(intersectingTiles, requestedBlock))
		readSingleTileIntoBlock(outputStream, intersectingTiles.front().second, warnings);
	else
		readMultipleTilesIntoBlock(outputStream, writer, intersectingTiles, warnings);
}

void IndexRasterBand::readSingleTileIntoBlock(std::ostream& outputStream, const IndexBlock& tile, IndexWarnings& warnings)
{
	auto dataFile = tile.getData(warnings);

	if (dataFile)
		outputStream << dataFile->rdbuf();
	else
		fillBlockWithUndefBigEndian(outputStream);
}

void IndexRasterBand::readMultipleTilesIntoBlock(std::ostream& outputStream, IndexTileWriter& writer, const std::vector<IndexBlocks::MapTile>& intersectingTiles, IndexWarnings& warnings)
{
	fillBlockWithUndefBigEndian(outputStream);

	for (const auto& touchedTile : intersectingTiles)
		writeTile(writer, touchedTile, warnings);
}

void IndexRasterBand::fillBlockWithUndefLittleEndianForException(std::ostream& outputStream)
{
	outputStream.clear();
	outputStream.seekp(std::ios::beg, 0);

	fillBlockWithUndefLittleEndian(outputStream);
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

GDALColorInterp IndexRasterBand::GetColorInterpretation()
{
	return GCI_GrayIndex;
}