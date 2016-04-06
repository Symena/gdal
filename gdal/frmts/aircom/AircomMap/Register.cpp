#include "gdal_priv.h"

#include "Dataset.h"

extern "C" void CPL_DLL GDALRegister_AIRCOM_MAP()
{
	GDALDriver* driver;
	if (!GDAL_CHECK_VERSION("AIRCOM_MAP"))
		return;
	if (GDALGetDriverByName("AIRCOM_MAP") == NULL)
	{
		driver = new GDALDriver();
		driver->SetDescription("AIRCOM_MAP");
		driver->SetMetadataItem(GDAL_DCAP_RASTER, "YES");
		driver->SetMetadataItem(GDAL_DMD_LONGNAME, "Aircom ENTERPRISE Map Data for height or clutter");

		driver->pfnOpen = aircom_map::Dataset::Open;
		
		GetGDALDriverManager()->RegisterDriver(driver);
	}
}
