#include "IndexDataset.h"

#include <iostream>

#include <boost/filesystem.hpp>

IndexDataset::~IndexDataset()
{

}

GDALDataset* IndexDataset::Open(GDALOpenInfo* openInfo)
{
	std::cout << "tried to open an index file\n";

	openInfo->pszFilename;

	std::cout << "in particular: " << boost::filesystem::absolute(openInfo->pszFilename) << "\n";

	return nullptr;
}
