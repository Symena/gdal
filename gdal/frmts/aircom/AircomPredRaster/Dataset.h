#pragma once

#include "gdal_pam.h"

#include "ApiWrapper.h"
#include "Warnings.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

class Dataset : public GDALPamDataset
{
	ApiWrapper apiWrapper; // must be initialized before geoParams

public:
	GeoParams geoParams;

	Dataset(const boost::filesystem::path& gapFile, Warnings& warnings);
	Dataset(std::wistream& gapFile, Warnings& warnings);
	Dataset(const boost::property_tree::wptree& gapTree, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);

	const auto& getBoundingBox() const { return geoParams.boundingBox; }
	double getResolution() const { return apiWrapper.getParams().predData.nResolution_cm / 100.0; }

	IPredRaster5Ptr getPredRaster() { return apiWrapper.getPredRaster(); }

	GDALDataType getSectionDataType(int sectionIndex);

private:
	void setBoundingBox();
};

}}
