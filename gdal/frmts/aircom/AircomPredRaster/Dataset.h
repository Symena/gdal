#pragma once

#include "gdal_priv.h"

#include "ApiWrapper.h"
#include "Warnings.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

class Dataset : public GDALDataset
{
	std::shared_ptr<ApiWrapper> apiWrapper;
	Auxiliary auxiliary;

public:
	Dataset(const boost::property_tree::wptree& gapTree, std::shared_ptr<ApiWrapper> apiWrapper, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);

	// loads auxiliary info from API if Auxiliary node is set to AutoComplete
	// modifies gapTree and writes modified tree to path
	static void autoCompleteAuxiliary(boost::property_tree::wptree& gapTree,
		const boost::filesystem::path& path, ApiWrapper& apiWrapper);

	virtual CPLErr GetGeoTransform(double* padfTransform) override;
	virtual const char* GetProjectionRef() override;

	const auto& getBoundingBox() const { return auxiliary.boundingBox; }
	double getResolution() const { return apiWrapper->getParams().predData.nResolution_cm / 100.0; }

private:
	void setBoundingBox();
};

}}
