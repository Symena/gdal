#include "IndexRegister.h"

#include "gdal_priv.h"

#include "IndexDataset.h"

void CPL_DLL GDALRegister_INDEX()
{
	GDALDriver  *driver;
	if (!GDAL_CHECK_VERSION("INDEX"))
		return;
	if (GDALGetDriverByName("INDEX") == NULL)
	{
		driver = new GDALDriver();
		driver->SetDescription("INDEX");
		driver->SetMetadataItem(GDAL_DCAP_RASTER, "YES");
		driver->SetMetadataItem(GDAL_DMD_LONGNAME, "ASSET Index Files for height or clutter data");
// 		driver->SetMetadataItem(GDAL_DCAP_VIRTUALIO, "YES"); does not seem necessary at the moment

		driver->pfnOpen = IndexDataset::Open;
		
		GetGDALDriverManager()->RegisterDriver(driver);
	}
}
