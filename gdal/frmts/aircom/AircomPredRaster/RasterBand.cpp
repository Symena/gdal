#include "RasterBand.h"

namespace aircom { namespace pred_raster {

RasterBand::RasterBand(Dataset* owningDataSet, int bandIndex, int section)
	: section(section)
{
	poDS = owningDataSet;
	nBand = bandIndex;
	eDataType = GDT_Int16;

	// nRasterXSize & nRasterYSize are set to the dataset size by GDALDataset::SetBand()

	// the size of each block and the number of blocks will be lazily initialized via
	// initializeBlocksInfo() to avoid unnecessary API accesses
	nBlockXSize = nBlockYSize = -1;
	nBlocksPerRow = nBlocksPerColumn = -1;

	SetNoDataValue(-9999);

	/*
	nBlocksPerRow = (nRasterXSize + nBlockXSize - 1) / nBlockXSize;    // ceiling(nRasterXSize / nBlockXSize)
	nBlocksPerColumn = (nRasterYSize + nBlockYSize - 1) / nBlockYSize; // ceiling(nRasterYSize / nBlockYSize)
	*/
}

GDALColorInterp RasterBand::GetColorInterpretation()
{
	return GDALColorInterp::GCI_GrayIndex;
}

CPLErr RasterBand::IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage)
{
	if (nBlockXSize == -1)
		throw std::logic_error("RasterBand::IReadBlock(): blocks info not initialized yet!");

	if (nXBlockOff < 0 || nXBlockOff >= nBlocksPerRow ||
		nYBlockOff < 0 || nYBlockOff >= nBlocksPerColumn ||
		pImage == nullptr)
	{
		return CPLErr::CE_Failure;
	}

	const size_t numPixels = size_t(nBlockXSize) * nBlockYSize;

	auto tile = getPredRaster()->CreateTileIterator(section)->GetTile(nXBlockOff, nYBlockOff);

	// TODO: verify that non-existing tiles are returned as null without HRESULT failure
	if (!tile)
	{
		// TODO: fill buffer with no data value?
		return CPLErr::CE_Failure;
	}

	if (numPixels != tile->GetPixelCount())
		throw std::logic_error("RasterBand::IReadBlock(): unexpected number of block pixels!");

	const auto numPixelsLong = static_cast<unsigned long>(numPixels);
	switch (eDataType)
	{
	case GDT_Byte:
		tile->GetUCharData(numPixelsLong, static_cast<unsigned char*>(pImage));
		break;
	case GDT_Int16:
		tile->GetShortData(numPixelsLong, static_cast<short*>(pImage));
		break;
	case GDT_UInt16:
		tile->GetUShortData(numPixelsLong, static_cast<unsigned short*>(pImage));
		break;
	case GDT_Int32:
		tile->GetIntData(numPixelsLong, static_cast<int*>(pImage));
		break;
	case GDT_UInt32:
		tile->GetUIntData(numPixelsLong, static_cast<unsigned int*>(pImage));
		break;
	case GDT_Float32:
		tile->GetFloatData(numPixelsLong, static_cast<float*>(pImage));
		break;
	case GDT_Float64:
		tile->GetDoubleData(numPixelsLong, static_cast<double*>(pImage));
		break;
	default:
		throw std::logic_error("RasterBand::IReadBlock(): unsupported GDAL data type!");
	}

	return CPLErr::CE_None;
}

}}
