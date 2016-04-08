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
}

GDALColorInterp RasterBand::GetColorInterpretation()
{
	return GDALColorInterp::GCI_GrayIndex;
}

CPLErr RasterBand::IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
	void * pData, int nBufXSize, int nBufYSize, GDALDataType eBufType, GSpacing nPixelSpace,
	GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg)
{
	if (nBlockXSize == -1)
		initializeBlocksInfo();

	return GDALPamRasterBand::IRasterIO(eRWFlag, nXOff, nYOff, nXSize, nYSize, pData,
		nBufXSize, nBufYSize, eBufType, nPixelSpace, nLineSpace, psExtraArg);
}

CPLErr RasterBand::IReadBlock(int, int, void*)
{
	return CPLErr::CE_Failure;
}

void RasterBand::initializeBlocksInfo()
{
	auto firstTile = getPredRaster()->CreateTileIterator(section)->GetNextTile();

	_REGIONEX tileRegion;
	firstTile->GetTileRegion(&tileRegion);

	nBlockXSize = tileRegion.m_width;
	nBlockYSize = tileRegion.m_height;

	nBlocksPerRow = (nRasterXSize + nBlockXSize - 1) / nBlockXSize;    // ceiling(nRasterXSize / nBlockXSize)
	nBlocksPerColumn = (nRasterYSize + nBlockYSize - 1) / nBlockYSize; // ceiling(nRasterYSize / nBlockYSize)
}

}}
