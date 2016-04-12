#pragma once

#include "gdal_pam.h"

#include "ApiWrapper.h"
#include "Warnings.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

class Dataset : public GDALPamDataset
{
	std::shared_ptr<ApiWrapper> apiWrapper;
	std::map<unsigned long, SectionInfo> sectionInfos;
	MapBox boundingBox;

public:
	Dataset(const boost::property_tree::wptree& gapTree, Warnings& warnings);
	Dataset(const boost::property_tree::wptree& gapTree, std::shared_ptr<ApiWrapper> apiWrapper, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);

	// Hull of section bounding boxes
	const auto& getBoundingBox() const { return boundingBox; }

	double getResolution() const { return apiWrapper->getParams().predData.nResolution_cm / 100.0; }

	IPredRaster5Ptr getPredRaster() { return apiWrapper->getPredRaster(); }

private:
	void setBoundingBox();
};

}}
