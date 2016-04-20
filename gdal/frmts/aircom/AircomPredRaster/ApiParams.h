#pragma once

#include "API.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

namespace aircom { namespace pred_raster {

enum class Section
{
	PathlossOnly = 0,
	InclinationOnly = 1,
	Unspecified
};

struct ApiParams
{
	// mandatory params
	boost::filesystem::path predictionFolder;
	PredData predData;
	std::wstring predAccessClassID;
	std::wstring predRasterClassID;

	// optional params
	Section section;

	ApiParams(const boost::filesystem::path& gapFilePath, const boost::filesystem::path& predictionFolder,
			  PredData predData, std::wstring predAccessClassID, std::wstring predRasterClassID, Section section);
	ApiParams(const boost::property_tree::wptree& apiNode, const boost::filesystem::path& gapFilePath);
};

}}