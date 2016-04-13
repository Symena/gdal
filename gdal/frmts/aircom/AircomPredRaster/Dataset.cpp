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

using SectionInfosMap = std::map<unsigned long, SectionInfo>;

SectionInfosMap parseOrLoadSectionInfos(const wptree& gapTree, ApiWrapper& wrapper, Warnings& warnings)
{
	auto sectionsNode = gapTree.get_child_optional(L"Sections");

	if (sectionsNode)
		try
		{
			SectionInfosMap ret;
			for (const auto& kv : sectionsNode.get())
			{
				auto sectionNum = boost::lexical_cast<unsigned long>(kv.first);
				ret.emplace(sectionNum, SectionInfo(kv.second));
			}
			return ret;
		}
		catch (const std::runtime_error& e)
		{
			std::string warning = format("Failed to load SectionInfos from json. Falling back to API. (%s)", e.what());
			warnings.add(warning);
		}

	return wrapper.getSectionInfos();
}

MapBox computeHull(const std::map<unsigned long, SectionInfo>& sectionInfos)
{
	MapBox hull({std::numeric_limits<int>::max(), std::numeric_limits<int>::max()},
				{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()});

	for (const auto& sectionInfo : sectionInfos)
	{
		auto& bottomLeft = sectionInfo.second.boundingBox.min_corner();
		auto& topRight = sectionInfo.second.boundingBox.max_corner();

		auto& hullBottomLeft = hull.min_corner();
		auto& hullTopRight = hull.max_corner();
		
		if (bottomLeft.get<0>() < hullBottomLeft.get<0>())
			hullBottomLeft.set<0>(bottomLeft.get<0>());

		if (bottomLeft.get<1>() < hullBottomLeft.get<1>())
			hullBottomLeft.set<1>(bottomLeft.get<1>());

		if (topRight.get<0>() > hullTopRight.get<0>())
			hullTopRight.set<0>(topRight.get<0>());

		if (topRight.get<1>() > hullTopRight.get<1>())
			hullTopRight.set<1>(topRight.get<1>());
	}
	
	return hull;
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
		ds->TryLoadXML();
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
	, sectionInfos(parseOrLoadSectionInfos(gapTree, *apiWrapper, warnings))
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

	auto requestedSection = apiWrapper->getParams().section;
	for (const auto& sectionInfoPair : sectionInfos)
	{
		const auto sectionNum = sectionInfoPair.first;
		const int bandIndex = sectionNum + 1;
		if (requestedSection == Section::Unspecified || static_cast<int>(requestedSection) == sectionNum)
			SetBand(bandIndex, new RasterBand(this, bandIndex, apiWrapper, sectionNum, sectionInfoPair.second));
	}
}

void Dataset::setBoundingBox()
{
	boundingBox = computeHull(sectionInfos);

	const double res = getResolution();
	nRasterXSize = static_cast<int>(std::round(width(boundingBox) / res));
	nRasterYSize = static_cast<int>(std::round(height(boundingBox) / res));

	if (nRasterXSize <= 0 || nRasterYSize <= 0)
		throw std::runtime_error(format("Invalid dimensions: %d x %d", nRasterXSize, nRasterYSize));

	double transformMatrix[6];
	transformMatrix[0] = boundingBox.min_corner().get<0>(); // minX
	transformMatrix[1] = res;
	transformMatrix[2] = 0;
	// top-down
	transformMatrix[3] = boundingBox.max_corner().get<1>(); // maxY
	transformMatrix[4] = 0;
	transformMatrix[5] = -res;

	SetGeoTransform(transformMatrix);
}

}}
