#include "gdal_priv.h"

extern "C" void CPL_DLL GDALRegister_AIRCOM_MAP();
extern "C" void CPL_DLL GDALRegister_AIRCOM_PRED_RASTER();

extern "C" void CPL_DLL GDALRegister_AIRCOM()
{
	GDALRegister_AIRCOM_MAP();
	GDALRegister_AIRCOM_PRED_RASTER();
}
