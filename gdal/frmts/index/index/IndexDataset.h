#pragma once

#include "gdal_pam.h"

class IndexDataset: public GDALPamDataset
{
public:
	~IndexDataset();

	static GDALDataset* Open(GDALOpenInfo* openInfo);
};
