#include "IndexRegister.h"

#include "gdal_priv.h"

#include "IndexDataset.h"

void CPL_DLL GDALRegister_INDEX()
{
	GDALDriver  *poDriver;
	if (!GDAL_CHECK_VERSION("INDEX"))
		return;
	if (GDALGetDriverByName("INDEX") == NULL)
	{
		poDriver = new GDALDriver();

		poDriver->SetDescription("INDEX");
		poDriver->SetMetadataItem(GDAL_DCAP_RASTER, "YES");
		poDriver->SetMetadataItem(GDAL_DMD_LONGNAME,
								  "Asset Index Files");
		poDriver->SetMetadataItem(GDAL_DCAP_VIRTUALIO, "YES");

		poDriver->pfnOpen = IndexDataset::Open;
		poDriver->pfnIdentify = nullptr;
		
		GetGDALDriverManager()->RegisterDriver(poDriver);
	}
}
