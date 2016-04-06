#include "Dataset.h"

#include <fstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace boost::property_tree;

namespace aircom_pred_raster {

namespace {

wptree loadJson(std::wistream& jsonStream)
{
	wptree tree;
	json_parser::read_json(jsonStream, tree);
	return tree;
}

aircom::PredData parsePredData(const wptree& predDataNode)
{
	aircom::PredData r;

	r.nX_cm = predDataNode.get<std::int64_t>(L"nX_cm");
	r.nY_cm = predDataNode.get<std::int64_t>(L"nY_cm");
	r.nAntennaHeight_cm = predDataNode.get<std::int32_t>(L"nAntennaHeight_cm");
	r.nGroundHeight_cm = predDataNode.get<std::int32_t>(L"nGroundHeight_cm");
	r.nResolution_cm = predDataNode.get<std::uint32_t>(L"nResolution_cm");
	r.nRadius_cm = predDataNode.get<std::uint32_t>(L"nRadius_cm");
	r.fFrequency_MHz = predDataNode.get<float>(L"fFrequency_MHz");
	r.nModelCRC = predDataNode.get<std::uint64_t>(L"nModelCRC");
	r.nPredFlags = predDataNode.get<std::uint32_t>(L"nPredFlags");

	r.nAntennaCRC = predDataNode.get<std::uint64_t>(L"nAntennaCRC");
	r.fAntennaMechanicalTilt_deg = predDataNode.get<double>(L"fAntennaMechanicalTilt_deg");
	r.fAntennaAzimuth_deg = predDataNode.get<double>(L"fAntennaAzimuth_deg");

	r.nCwWeight = predDataNode.get<std::uint16_t>(L"nCwWeight");
	r.fCwRolloff = predDataNode.get<float>(L"fCwRolloff");

	return r;
}

Sections parseSections(std::wstring sections)
{
	boost::to_lower(sections);

	if (sections == L"pathloss")
		return Sections::PathlossOnly;
	if (sections == L"InclinationOnly")
		return Sections::InclinationOnly;

	return Sections::Both;
}

}

Dataset::Dataset(const boost::filesystem::path& gapFile, Warnings& warnings)
	: Dataset(std::wifstream(gapFile.string()), warnings)
{}

Dataset::Dataset(std::wistream& gapFile, Warnings& warnings)
	: Dataset(loadJson(gapFile), warnings)
{}

Dataset::Dataset(const wptree& gapTree, Warnings& warnings)
{
	const auto guid = gapTree.get<std::wstring>(L"API.CRasterGUID");
	// TODO: set up appropriate COM factory

	predictionFolder = gapTree.get<std::wstring>(L"API.PredictionFolder");
	predData = parsePredData(gapTree.get_child(L"API.PredData"));
	sections = parseSections(gapTree.get<std::wstring>(L"API.Sections", L""));
}

}