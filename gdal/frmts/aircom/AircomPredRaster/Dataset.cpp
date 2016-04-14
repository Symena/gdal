#include "Dataset.h"

#include <fstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "WarningsReporter.h"
#include "RasterBand.h"

using namespace boost::property_tree;

namespace aircom { namespace pred_raster {

namespace {

wptree loadJson(const boost::filesystem::path& path)
{
	std::wifstream jsonStream(path.string());
	wptree tree;
	json_parser::read_json(jsonStream, tree);
	return tree;
}

Auxiliary parseOrLoadAuxiliary(const wptree& gapTree, ApiWrapper& wrapper, Warnings& warnings)
{
	auto auxiliaryNode = gapTree.get_child_optional(L"Auxiliary");

	if (auxiliaryNode)
		try
		{
			return Auxiliary(auxiliaryNode.get());
		}
		catch (const std::runtime_error& e)
		{
			std::string warning = format("Failed to load auxiliary info from json. Falling back to API. (%s)", e.what());
			warnings.add(warning);
		}

	return wrapper.getAuxiliary();
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
		auto gapTree = loadJson(path);
		auto ds = std::make_unique<Dataset>(gapTree, warnings);
		ds->SetDescription(openInfo->pszFilename);
		ds->oOvManager.Initialize(ds.get(), openInfo->pszFilename);
		return ds.release();
	}
	catch (const boost::property_tree::json_parser_error&)
	{
		warnings.add("File has a .gap extension but is no valid JSON file, so not suited for Aircom ENTERPRISE Prediction driver");
	}
	catch (const std::exception& e)
	{
		warnings.add("File has a .gap extension but Aircom ENTERPRISE Prediction driver failed to load it: %s", e.what());
	}

	return nullptr;
}

Dataset::Dataset(const wptree& gapTree, Warnings& warnings)
	: Dataset
	( gapTree
	, std::make_shared<ApiWrapper>(ApiParams(gapTree.get_child(L"EnterprisePredRasterApi")))
	, warnings)
{}

Dataset::Dataset(const wptree& gapTree, std::shared_ptr<ApiWrapper> tmpApiWrapper, Warnings& warnings)
	: apiWrapper(std::move(tmpApiWrapper))
	, auxiliary(parseOrLoadAuxiliary(gapTree, *apiWrapper, warnings))
{
	setBoundingBox();

	auto meta = gapTree.get_child_optional(L"Meta");
	if (meta)
	{
		for (auto& metaDomain : meta.get())
		{
			auto domain = ws2ns(metaDomain.first);
			for (auto& kv : metaDomain.second)
			{
				auto key = ws2ns(kv.first);
				auto value = ws2ns(kv.second.data());
				SetMetadataItem(key.c_str(), value.c_str(), domain.c_str());
			}
		}
	}

	const auto requestedSection = apiWrapper->getParams().section;
	const MapPoint sizeInPixels = { nRasterXSize, nRasterYSize };

	for (const auto& sectionPair : auxiliary.sectionDataTypes)
	{
		const auto sectionNum = sectionPair.first;
		const int bandIndex = sectionNum + 1;
		if (requestedSection == Section::Unspecified || static_cast<int>(requestedSection) == sectionNum)
			SetBand(bandIndex, new RasterBand(this, sizeInPixels, bandIndex, apiWrapper, sectionNum, auxiliary));
	}
}

CPLErr Dataset::SetGeoTransform(double* padfTransform)
{
	return CPLErr::CE_Failure;
}

CPLErr Dataset::GetGeoTransform(double* padfTransform)
{
	if (!padfTransform)
		return CPLErr::CE_Failure;

	const double res = getResolution();

	padfTransform[0] = getBoundingBox().min_corner().get<0>(); // min x
	padfTransform[1] = res;
	padfTransform[2] = 0;
	// top-down
	padfTransform[3] = getBoundingBox().max_corner().get<1>(); // max y
	padfTransform[4] = 0;
	padfTransform[5] = -res;

	return CPLErr::CE_None;
}

void Dataset::setBoundingBox()
{
	const double res = getResolution();
	nRasterXSize = static_cast<int>(std::round(width(getBoundingBox()) / res));
	nRasterYSize = static_cast<int>(std::round(height(getBoundingBox()) / res));

	if (nRasterXSize <= 0 || nRasterYSize <= 0)
		throw std::runtime_error(format("Invalid dimensions: %d x %d", nRasterXSize, nRasterYSize));
}

}}
