#include "Dataset.h"

#include <fstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "membuf.h"
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
	if (sections == L"InclinationOnly")
		return Sections::InclinationOnly;

	return Sections::Both;
}

}

GDALDataset* Dataset::Open(GDALOpenInfo* openInfo)
{
	if (openInfo->pszFilename == nullptr || openInfo->pabyHeader == nullptr || openInfo->fpL == nullptr)
		return nullptr;

	if (!openInfo->bStatOK)
		return nullptr;

	if (openInfo->eAccess != GA_ReadOnly)
	{
		CPLError(CE_Failure, CPLE_NotSupported, "The Aircom ENTERPRISE Map Data driver only supports readonly access to existing datasets.\n");
		return nullptr;
	}

	const boost::filesystem::path path = openInfo->pszFilename;

	membuf sbuf(reinterpret_cast<char*>(openInfo->pabyHeader), openInfo->nHeaderBytes);
	std::istream header(&sbuf);

	/* TODO
	if (!Identify(indexFile, header))
		return nullptr;
	*/

	Warnings warnings;
	WarningsReporter warningsReporter(warnings);

	std::unique_ptr<Dataset> dataSet;

	try
	{
		WarningsContext context(warnings, boost::filesystem::absolute(path).string() + ": ");
		dataSet = std::make_unique<Dataset>(path, warnings);
	}
	catch (const std::runtime_error& e)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Reading index file %s failed: %s", openInfo->pszFilename, e.what());
		return nullptr;
	}

	dataSet->SetDescription(openInfo->pszFilename);
	dataSet->TryLoadXML();

	dataSet->oOvManager.Initialize(dataSet.get(), openInfo->pszFilename);

	return dataSet.release();
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

}}
