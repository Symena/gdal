#include "gdal_priv.h"

#include "Dataset.h"

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
		
		GetGDALDriverManager()->RegisterDriver(driver);
	}
}
