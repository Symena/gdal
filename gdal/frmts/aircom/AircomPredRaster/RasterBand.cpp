#include "RasterBand.h"

namespace aircom { namespace pred_raster {

namespace {

boost::optional<double> getNoDataValue(const SectionInfo& sectionInfo)
{
	switch (sectionInfo.dataType)
	{
	case GDT_Float32:
		return -9999; // at least for unmasked loss according to docs
	case GDT_Int16:
		return -9999; // at least for unmasked angles
	case GDT_Byte:
		return 200;   // at least for masked loss and masked+unmasked LineOfSightInfo
	}

	return boost::none;
}

}

RasterBand::RasterBand(Dataset* owningDataSet, int bandIndex, std::shared_ptr<ApiWrapper> tmpApiWrapper,
	unsigned long sectionNum, const SectionInfo& sectionInfo)
	: apiWrapper(std::move(tmpApiWrapper))
	, sectionNum(sectionNum)
{
	poDS = owningDataSet;
	nBand = bandIndex;
	eDataType = sectionInfo.dataType;

	nBlockXSize = sectionInfo.tileSizeInPixels.get<0>();
	nBlockYSize = sectionInfo.tileSizeInPixels.get<1>();

	nBlocksPerRow = sectionInfo.numTiles.get<0>();
	nBlocksPerColumn = sectionInfo.numTiles.get<1>();

	// nRasterXSize & nRasterYSize are set to the dataset size by GDALDataset::SetBand()

	if (auto noDataValue = getNoDataValue(sectionInfo))
		SetNoDataValue(*noDataValue);
}

GDALColorInterp RasterBand::GetColorInterpretation()
{
	return GDALColorInterp::GCI_GrayIndex;
}

CPLErr RasterBand::IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage)
{
	if (nXBlockOff < 0 || nXBlockOff >= nBlocksPerRow ||
		nYBlockOff < 0 || nYBlockOff >= nBlocksPerColumn ||
		pImage == nullptr)
	{
		return CPLErr::CE_Failure;
	}

	const size_t numPixels = size_t(nBlockXSize) * nBlockYSize;

	auto tileIterator = getPredRaster()->CreateTileIterator(sectionNum);

	IRasterTilePtr tile;
	if (FAILED(tileIterator->raw_GetTile(nXBlockOff, nYBlockOff, &tile)))
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
