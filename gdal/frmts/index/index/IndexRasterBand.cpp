#include "IndexRasterBand.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include "IndexLine.h"

IndexRasterBand::IndexRasterBand(IndexDataset* owningDataSet, const IndexLine& line)
	: referencedFile(line.getTilePath())
{
	this->poDS = owningDataSet;
	this->nBand = 1;

	eDataType = GDT_Int16;

	nBlockXSize = (line.getTileEastMax() - line.getTileEastMin()) / line.getPixelSquareSize();
	nBlockYSize = (line.getTileNorthMax() - line.getTileNorthMin()) / line.getPixelSquareSize();
}

CPLErr IndexRasterBand::IReadBlock(int nBlockXOff, int nBlockYOff, void* pImage)
{
	assert(!nBlockXOff);
	assert(!nBlockYOff);

// 	auto dataPuffer = static_cast<std::int16_t*>(pImage);

	std::ifstream dataFile(referencedFile.string(), std::ios::binary);

	if(!dataFile)
	{
		CPLError(CE_Failure, CPLE_AppDefined, ("Could not open tile file "+ boost::filesystem::absolute(referencedFile).string()).c_str());
		return CE_Failure;
	}

	auto actualFileSize = boost::filesystem::file_size(referencedFile);
	auto expectedFileSize = sizeof(std::int16_t) * this->nBlockXSize * this->nBlockYSize;

	if(actualFileSize != expectedFileSize)
	{
		CPLError(CE_Failure, CPLE_AppDefined, (boost::filesystem::absolute(referencedFile).string()+" has unexpected size").c_str());
		return CE_Failure;
	}

	dataFile.read((char*)pImage, actualFileSize);

	return CE_None;
}

