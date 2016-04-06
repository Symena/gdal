#pragma once

#include "gdal_pam.h"

#include "API.h"
#include "Geometry.h"
#include "Warnings.h"

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

class ComFactory;

enum class Sections
{
	PathlossOnly,
	InclinationOnly,
	Unspecified
};

class Dataset : public GDALPamDataset
{
public:
	Dataset(const boost::filesystem::path& gapFile, Warnings& warnings);
	Dataset(std::wistream& gapFile, Warnings& warnings);
	Dataset(const boost::property_tree::wptree& gapTree, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);

	const auto& getBoundingBox() const { return boundingBox; }
	double getResolution() const { return predData.nResolution_cm / 100.0; }

private:
	boost::filesystem::path predictionFolder;
	PredData predData;
	Sections sections;

	std::unique_ptr<ComFactory> comFactory;
	MapBox boundingBox;
};

}}
