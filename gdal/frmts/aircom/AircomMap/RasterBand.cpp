#include "RasterBand.h"

namespace aircom_map {

RasterBand::RasterBand(Dataset* owningDataSet, int bandIndex)
{
	poDS = owningDataSet;
	nBand = bandIndex;
	eDataType = GDT_Int16;

	// nRasterXSize & nRasterYSize are set to the dataset size by GDALDataset::SetBand()

	SetNoDataValue(-9999);
}

char** RasterBand::GetCategoryNames()
{
	auto& clutterCodes = getDataset().getClutterCodes();
	if (clutterCodes)
		return clutterCodes->getClutterCodes();

	return nullptr;
}

GDALColorInterp RasterBand::GetColorInterpretation()
{
	return GDALColorInterp::GCI_GrayIndex;
}

CPLErr RasterBand::IReadBlock(int, int, void*)
{
	return CPLErr::CE_Failure;
}

CPLErr RasterBand::IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
	void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
	GSpacing nPixelSpace, GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg)
{
	return getDataset().RasterIO(eRWFlag,
		nXOff, nYOff, nXSize, nYSize,
		pData, nBufXSize, nBufYSize, eBufType,
		1, &nBand,
		nPixelSpace, nLineSpace, 0,
		psExtraArg);
}

}
