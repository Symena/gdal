#include "gdal_priv.h"

#include "Dataset.h"
#include "PredRasterFactory.h"

namespace {

void unloadDriver(GDALDriver*)
{
	aircom::pred_raster::PredRasterFactory::cleanup();
}

}

extern "C" void CPL_DLL GDALRegister_AIRCOM_PRED_RASTER()
{
	GDALDriver* driver;
	if (!GDAL_CHECK_VERSION("AIRCOM_PRED_RASTER"))
		return;
	if (GDALGetDriverByName("AIRCOM_PRED_RASTER") == NULL)
	{
		driver = new GDALDriver();
		driver->SetDescription("AIRCOM_PRED_RASTER");
		driver->SetMetadataItem(GDAL_DCAP_RASTER, "YES");
		driver->SetMetadataItem(GDAL_DMD_LONGNAME, "Aircom ENTERPRISE Prediction Data");

		driver->pfnOpen = aircom::pred_raster::Dataset::Open;
		driver->pfnUnloadDriver = unloadDriver;

		GetGDALDriverManager()->RegisterDriver(driver);
	}
}
