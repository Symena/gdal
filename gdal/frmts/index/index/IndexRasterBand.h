#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include "gdal_pam.h"

#include "IndexDataset.h"
#include "IndexBlocks.h"
#include "IndexClutterCodes.h"

class IndexTileWriter;

class IndexRasterBand: public GDALPamRasterBand
{
	IndexBlocks blocks;

	std::vector<std::int16_t> undefValueLineBigEndian;
	std::vector<std::int16_t> undefValueLineLittleEndian;

	boost::optional<IndexClutterCodes> clutterCodes;

public:
	IndexRasterBand(IndexDataset* owningDataSet, IndexBlocks blocks, boost::optional<IndexClutterCodes> codes);
	
	virtual CPLErr IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage) override;

	virtual char** GetCategoryNames() override;

	virtual GDALColorInterp GetColorInterpretation() override;

private:
	void readBlock(int nBlockXOff, int nBlockYOff, std::ostream& outputStream, std::int16_t* outputData);

	void readTilesIntoBlock(int nBlockXOff, int nBlockYOff, std::ostream& outputStream, std::int16_t* outputData, IndexWarnings& warnings);
	/*
	void readIntersectingTilesIntoBlock(std::ostream& outputStream, std::int16_t* outputData, const std::vector<IndexBlocks::MapTile>& intersectingTiles, const MapBox& requestedBlock, IndexWarnings& warnings);
	void readSingleTileIntoBlock(std::ostream& outputStream, std::int16_t* outputData, const IndexBlock& tile, IndexWarnings& warnings);
	void readMultipleTilesIntoBlock(std::ostream& outputStream, IndexTileWriter& writer, const std::vector<IndexBlocks::MapTile>& intersectingTiles, IndexWarnings& warnings);

	bool singleTileMatchesBlockPerfectly(const std::vector<IndexBlocks::MapTile>& intersectingTiles, const MapBox& requestedBlock);
	*/

	void fillBlockWithUndefLittleEndian(std::ostream& outputStream);
	void fillBlockWithUndefBigEndian(std::ostream& outputStream);
	void fillBlockWithUndefLittleEndianForException(std::ostream& outputStream);

	void fillBlock(std::ostream& outputStream, const std::vector<std::int16_t>& lineData);

	void convertToNativeByteOrder(std::int16_t* outputData);
};
