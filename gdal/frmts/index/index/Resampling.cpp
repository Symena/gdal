#include "Resampling.h"

#include "../frmts/mem/memdataset.h"
#include <cassert>
#include <boost/optional.hpp>

void resample(const void* src, int srcWidth, int srcHeight,
              void* dst, int dstWidth, int dstHeight,
              GDALDataType pixelType, GDALRIOResampleAlg algorithm,
              boost::optional<double> noDataValue)
{
	assert(src && "No source");
	assert(dst && "No destination");

	auto dataset = MEMDataset::Create(nullptr, srcWidth, srcHeight, 0, pixelType, nullptr);

	// add a band wrapping the source buffer
	char dataPointerOption[64];
	snprintf(dataPointerOption, sizeof(dataPointerOption), "DATAPOINTER=0x%p", src);
	char* options[] = { dataPointerOption, nullptr };
	if (dataset->AddBand(pixelType, options) != CPLErr::CE_None)
		throw std::runtime_error("resample(): failed to add RasterBand to MEMDataset");
	auto band = dataset->GetRasterBand(1);

	if (noDataValue.is_initialized())
		band->SetNoDataValue(noDataValue.get());

	GDALRasterIOExtraArg extraArg = { 0 }; // zero-initialize whole struct
	extraArg.nVersion = 1;
	extraArg.eResampleAlg = algorithm;

	if (band->RasterIO(GDALRWFlag::GF_Read, 0, 0, srcWidth, srcHeight,
	                   dst, dstWidth, dstHeight, pixelType, 0, 0, &extraArg) != CPLErr::CE_None)
		throw std::runtime_error("resample(): resampling failed");
}

void resample(const void* src, int srcWidth, int srcHeight,
              void* dst, int dstWidth, int dstHeight,
              GDALDataType pixelType, GDALRIOResampleAlg algorithm)
{
	return resample(src, srcWidth, srcHeight, dst, dstWidth, dstHeight,
		pixelType, algorithm, boost::optional<double>());
}

void resample(const void* src, int srcWidth, int srcHeight,
              void* dst, int dstWidth, int dstHeight,
              GDALDataType pixelType, GDALRIOResampleAlg algorithm,
              double noDataValue)
{
	return resample(src, srcWidth, srcHeight, dst, dstWidth, dstHeight,
		pixelType, algorithm, boost::optional<double>(noDataValue));
}
