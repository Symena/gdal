#include "IndexRasterBand.h"

IndexRasterBand::IndexRasterBand(IndexDataset* owningDataSet, int bandIndex, int resolution)
{
	poDS = owningDataSet;
	nBand = bandIndex;

	auto alignCoord = [resolution](int x, bool floor)
	{
		double exact = static_cast<double>(x) / resolution;
		return static_cast<int>(floor ? std::floor(exact) : std::ceil(exact)) * resolution;
	};

	// align the bounding box at multiples of the resolution, expanding it if need be
	// (=> absolute origin (0, 0) will be a pixel corner)
	// this matches the alignment requirements for blocks of this resolution
	const auto& dsBounds = owningDataSet->getBoundingBox();
	bounds = makeBox(
		alignCoord(dsBounds.min_corner().get<0>(), true),
		alignCoord(dsBounds.min_corner().get<1>(), true),
		alignCoord(dsBounds.max_corner().get<0>(), false),
		alignCoord(dsBounds.max_corner().get<1>(), false));
	this->resolution = resolution;

	nRasterXSize = width(bounds) / resolution;
	nRasterYSize = height(bounds) / resolution;

	eDataType = GDT_Int16;
	SetNoDataValue(-9999);
}

char** IndexRasterBand::GetCategoryNames()
{
	auto& clutterCodes = getDataset().getClutterCodes();
	if (clutterCodes)
		return clutterCodes->getClutterCodes();

	return nullptr;
}

GDALColorInterp IndexRasterBand::GetColorInterpretation()
{
	return GCI_GrayIndex;
}

CPLErr IndexRasterBand::IReadBlock(int, int, void*)
{
	return CPLErr::CE_Failure;
}

CPLErr IndexRasterBand::IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
	void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
	GSpacing nPixelSpace, GSpacing nLineSpace, GDALRasterIOExtraArg* psExtraArg)
{
	if (eRWFlag != GDALRWFlag::GF_Read)
	{
		CPLError(CPLErr::CE_Failure, CPLE_NoWriteAccess, "Index raster bands can only be read from");
		return CPLErr::CE_Failure;
	}

	if (nXSize <= 0 || nYSize <= 0 || nBufXSize <= 0 || nBufYSize <= 0 || pData == nullptr)
	{
		CPLError(CPLErr::CE_Failure, CPLE_IllegalArg, "Invalid arguments");
		return CPLErr::CE_Failure;
	}

	if (eBufType != eDataType ||
		(nPixelSpace != 0 && nPixelSpace != sizeof(std::int16_t)) ||
		(nLineSpace != 0 && nLineSpace != nBufXSize * nPixelSpace))
	{
		CPLError(CPLErr::CE_Failure, CPLE_NotSupported, "Index raster bands only support reading into contiguous 16-bit buffers");
		return CPLErr::CE_Failure;
	}

	if (nBufXSize != nXSize || nBufYSize != nYSize)
	{
		CPLError(CPLErr::CE_Failure, CPLE_NotSupported, "Index raster bands can only be read from in their native resolution. Use the data set for arbitrary resolutions.");
		return CPLErr::CE_Failure;
	}

	auto algorithm = (psExtraArg ? psExtraArg->eResampleAlg : GDALRIOResampleAlg::GRIORA_NearestNeighbour);
	bool success = getDataset().render(static_cast<std::int16_t*>(pData), nBufXSize, nBufYSize, resolution,
		bounds.min_corner() + MapPoint(nXOff * resolution, nYOff * resolution), algorithm, algorithm);

	return success ? CPLErr::CE_None : CPLErr::CE_Failure;
}
