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
		poDriver->SetMetadataItem(GDAL_DMD_LONGNAME, "Asset Index Files for height or clutter data");
// 		poDriver->SetMetadataItem(GDAL_DCAP_VIRTUALIO, "YES"); does not seem necessary at the moment

		poDriver->pfnOpen = IndexDataset::Open;
		
		GetGDALDriverManager()->RegisterDriver(poDriver);
	}
}
