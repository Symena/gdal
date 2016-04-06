#include "Dataset.h"

#include <fstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "ComFactory.h"
#include "WarningsReporter.h"

using namespace boost::property_tree;

namespace aircom { namespace pred_raster {

namespace {

wptree loadJson(std::wistream& jsonStream)
{
	wptree tree;
	json_parser::read_json(jsonStream, tree);
	return tree;
}

PredData parsePredData(const wptree& predDataNode)
{
	PredData r;

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
	if (sections == L"inclination")
		return Sections::InclinationOnly;

	return Sections::Unspecified;
}

}

GDALDataset* Dataset::Open(GDALOpenInfo* openInfo)
{
	if (openInfo->pszFilename == nullptr || !openInfo->bStatOK)
		return nullptr;

	if (openInfo->eAccess != GA_ReadOnly)
	{
		CPLError(CE_Failure, CPLE_NotSupported, "The Aircom ENTERPRISE Prediction driver only supports readonly access to existing datasets.\n");
		return nullptr;
	}

	const boost::filesystem::path path = openInfo->pszFilename;
	const auto lowerExtension = boost::to_lower_copy(path.extension().wstring());
	if (lowerExtension != L".gap")
		return nullptr;

	Warnings warnings;
	WarningsReporter warningsReporter(warnings);
	WarningsContext context(warnings, boost::filesystem::absolute(path).string() + ": ");

	try
	{
		auto ds = std::make_unique<Dataset>(path, warnings);
		ds->SetDescription(openInfo->pszFilename);
		ds->TryLoadXML();
		ds->oOvManager.Initialize(ds.get(), openInfo->pszFilename);
		return ds.release();
	}
	catch (const boost::property_tree::json_parser_error&)
	{
		warnings.add("File has a .gap extension but is no valid JSON file, so not suited for Aircom ENTERPRISE Prediction driver");
	}
	catch (const std::exception&)
	{
		warnings.add("File has a .gap extension but Aircom ENTERPRISE Prediction driver failed to load it");
	}

	return nullptr;
}

Dataset::Dataset(const boost::filesystem::path& gapFile, Warnings& warnings)
	: Dataset(std::wifstream(gapFile.string()), warnings)
{}

Dataset::Dataset(std::wistream& gapFile, Warnings& warnings)
	: Dataset(loadJson(gapFile), warnings)
{}

Dataset::Dataset(const wptree& gapTree, Warnings& warnings)
{
	const auto& apiNode = gapTree.get_child(L"API");

	predictionFolder = apiNode.get<std::wstring>(L"PredictionFolder");
	predData = parsePredData(apiNode.get_child(L"PredData"));
	sections = parseSections(apiNode.get<std::wstring>(L"Section", L""));

	const auto predRasterClassIDString = apiNode.get<std::wstring>(L"CRasterGUID");
	comFactory = std::make_unique<ComFactory>(predRasterClassIDString);
}

}}
