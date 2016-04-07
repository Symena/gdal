#pragma once

#include "API.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

enum class Section
{
	PathlossOnly,
	InclinationOnly,
	Unspecified
};

struct ApiParams
{
	// mandatory params
	boost::filesystem::path predictionFolder;
	PredData predData;
	std::wstring cRasterGUID;

	// optional params
	Section section;

	explicit ApiParams(const boost::property_tree::wptree& apiNode);
};

}}