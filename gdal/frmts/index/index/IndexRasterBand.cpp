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
	char* imageBuffer = static_cast<char*>(pImage);

	auto optionalBlock = blocks.getBlock(nBlockXOff, nBlockYOff);

	if(!optionalBlock)
		return CE_None;

	const auto& accessedBlock = *optionalBlock;

	const auto& blockFile = accessedBlock.getFile();

	std::ifstream dataFile(blockFile.string(), std::ios::binary);

	if(!dataFile)
	{
		//TODO: write unknown value here
		CPLError(CE_Warning, CPLE_AppDefined, ("Could not open tile file "+ boost::filesystem::absolute(blockFile).string()).c_str());
		return CE_None;
	}

	auto actualFileSize = boost::filesystem::file_size(blockFile);
	auto expectedFileSize = sizeof(std::int16_t) * accessedBlock.getRasterSizeX() * accessedBlock.getRasterSizeY();

	if(actualFileSize != expectedFileSize)
	{
		CPLError(CE_Failure, CPLE_AppDefined, (boost::filesystem::absolute(blockFile).string()+" has unexpected size").c_str());
		return CE_Failure;
	}

	if(nBlockXSize == accessedBlock.getRasterSizeX() && nBlockYSize == accessedBlock.getRasterSizeY())
		dataFile.read(imageBuffer, actualFileSize);
	else
	{
		auto bytesOfOneActualLine = sizeof(std::int16_t) * accessedBlock.getRasterSizeX();
		auto bytesOfExpectedLine = sizeof(std::int16_t) * nBlockXSize;

		assert(bytesOfExpectedLine <= blocks.getUndefBlockLine().size()*sizeof(std::int16_t));

		auto undefValueDataPtr = static_cast<const void*>(blocks.getUndefBlockLine().data());

		for (int i = 0; i < accessedBlock.getOffsetInBlockY();++i)
		{
			memcpy(imageBuffer, undefValueDataPtr, bytesOfExpectedLine);
			imageBuffer += bytesOfExpectedLine;
		}

		memcpy(imageBuffer, undefValueDataPtr, sizeof(std::int16_t) * accessedBlock.getOffsetInBlockX());
		imageBuffer += sizeof(std::int16_t) * accessedBlock.getOffsetInBlockX();

		auto bytesToSkip = bytesOfExpectedLine - bytesOfOneActualLine;

		size_t readBytes = 0;

		assert(actualFileSize % bytesOfOneActualLine == 0);

		while(readBytes != actualFileSize)
		{
			dataFile.read(imageBuffer, bytesOfOneActualLine);
			if (dataFile.gcount() != bytesOfOneActualLine)
				throw std::runtime_error("could not read complete file");

			imageBuffer += bytesOfOneActualLine;
			readBytes += bytesOfOneActualLine;

			if (readBytes < actualFileSize)
			{
				memcpy(imageBuffer, undefValueDataPtr, bytesToSkip);
				imageBuffer += bytesToSkip;
			}
		}

		if(accessedBlock.getOffsetInBlockX() == 0)
		{
			memcpy(imageBuffer, undefValueDataPtr, bytesToSkip);
			imageBuffer += bytesToSkip;
		}

		const auto remainingLines = nBlockYSize - accessedBlock.getOffsetInBlockY() - accessedBlock.getRasterSizeY();
		for (int i = 0; i < remainingLines; ++i)
		{
			memcpy(imageBuffer, undefValueDataPtr, bytesOfExpectedLine);
			imageBuffer += bytesOfExpectedLine;
		}

		auto expectedImageBuffer = static_cast<char*>(pImage) + nBlockXSize * nBlockYSize * sizeof(std::int16_t);

		if(imageBuffer != expectedImageBuffer)
			throw std::logic_error("Have not written the exact amount of data");
	}

	return CE_None;
}

GDALColorInterp IndexRasterBand::GetColorInterpretation()
{
	return GCI_GrayIndex;
}

