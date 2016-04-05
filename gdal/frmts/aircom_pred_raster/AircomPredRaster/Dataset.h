#pragma once

#include "gdal_pam.h"

#include "Aircom/PredData.h"
#include "../../aircom_map/AircomMap/Warnings.h"
#include "../../aircom_map/AircomMap/Geometry.h"

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

class Dataset : public GDALPamDataset
{
	aircom_map::MapBox boundingBox;
	aircom::PredData predData;
	boost::filesystem::path predictionFolder;

public:
	using Warnings = aircom_map::Warnings;

	Dataset(const boost::filesystem::path& gapFile, Warnings& warnings);
	Dataset(std::wistream& gapFile, Warnings& warnings);
	Dataset(const boost::property_tree::wptree& gapTree, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);
	static bool Identify(const boost::filesystem::path& file, std::istream& header);

	const auto& getBoundingBox() const { return boundingBox; }
	double getResolution() const { return predData.nResolution_cm / 100.0; }
};

}}