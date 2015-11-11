#pragma once

#include "gdal.h"

void resample(const void* src, int srcWidth, int srcHeight,
              void* dst, int dstWidth, int dstHeight,
              GDALDataType pixelType, GDALRIOResampleAlg algorithm);

void resample(const void* src, int srcWidth, int srcHeight,
              void* dst, int dstWidth, int dstHeight,
              GDALDataType pixelType, GDALRIOResampleAlg algorithm,
              double noDataValue);
