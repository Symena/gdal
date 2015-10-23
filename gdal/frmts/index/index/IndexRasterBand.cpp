#include "IndexRasterBand.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include "IndexLine.h"

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
	const auto& accessedBlock = *blocks.getBlock(nBlockXOff, nBlockYOff);

	const auto& blockFile = accessedBlock.getFile();

	std::ifstream dataFile(blockFile.string(), std::ios::binary);

	if(!dataFile)
	{
		CPLError(CE_Failure, CPLE_AppDefined, ("Could not open tile file "+ boost::filesystem::absolute(blockFile).string()).c_str());
		return CE_Failure;
	}

	auto actualFileSize = boost::filesystem::file_size(blockFile);
	auto expectedFileSize = sizeof(std::int16_t) * this->nBlockXSize * this->nBlockYSize;

	if(actualFileSize != expectedFileSize)
	{
		CPLError(CE_Failure, CPLE_AppDefined, (boost::filesystem::absolute(blockFile).string()+" has unexpected size").c_str());
		return CE_Failure;
	}

	dataFile.read((char*)pImage, actualFileSize);

	return CE_None;
}

