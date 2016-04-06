#include "RasterBand.h"

namespace aircom { namespace pred_raster {

RasterBand::RasterBand(Dataset* owningDataSet, int bandIndex)
{
	poDS = owningDataSet;
	nBand = bandIndex;
	eDataType = GDT_Int16;

	// nRasterXSize & nRasterYSize are set to the dataset size by GDALDataset::SetBand()

	SetNoDataValue(-9999);
}

GDALColorInterp RasterBand::GetColorInterpretation()
{
	return GDALColorInterp::GCI_GrayIndex;
}

CPLErr RasterBand::IReadBlock(int, int, void*)
{
	return CPLErr::CE_Failure;
}

}}
