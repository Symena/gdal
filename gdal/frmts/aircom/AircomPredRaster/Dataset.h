#pragma once

#include "gdal_pam.h"

#include "Aircom/PredData.h"
#include "Warnings.h"
#include "Geometry.h"

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

enum class Sections
{
	PathlossOnly,
	InclinationOnly,
	Both
};

class Dataset : public GDALPamDataset
{
public:
	Dataset(const boost::filesystem::path& gapFile, Warnings& warnings);
	Dataset(std::wistream& gapFile, Warnings& warnings);
	Dataset(const boost::property_tree::wptree& gapTree, Warnings& warnings);

	static GDALDataset* Open(GDALOpenInfo* openInfo);
	static bool Identify(const boost::filesystem::path& file, std::istream& header);

	const auto& getBoundingBox() const { return boundingBox; }
	double getResolution() const { return predData.nResolution_cm / 100.0; }

private:
	boost::filesystem::path predictionFolder;
	PredData predData;
	Sections sections;

	MapBox boundingBox;
};

}}
