#include "Dataset.h"

#include <fstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "WarningsReporter.h"
#include "RasterBand.h"

using namespace boost::property_tree;

namespace aircom { namespace pred_raster {

namespace {

wptree loadJson(std::wistream& jsonStream)
{
	wptree tree;
	json_parser::read_json(jsonStream, tree);
	return tree;
}

GeoParams parseOrLoadGeoParams(const wptree& gapTree, ApiWrapper& wrapper, Warnings& warnings)
{
	auto geoNode = gapTree.get_child_optional(L"Geo");
	if (geoNode)	
		try
		{
			return GeoParams(geoNode.get());
		}
		catch (std::runtime_error e)
		{
			std::string warning = format("Failed to load GeoParams from json node. Falling back to API. (%s)", e.what());
			warnings.add(warning);
		}

	return wrapper.getGeoParams();
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
	catch (const std::exception& e)
	{
		warnings.add("File has a .gap extension but Aircom ENTERPRISE Prediction driver failed to load it: %s", e.what());
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
	: apiWrapper(ApiParams(gapTree.get_child(L"EnterprisePredRasterApi")))
	, geoParams(parseOrLoadGeoParams(gapTree, apiWrapper, warnings))
{
	setBoundingBox();

	if (nRasterXSize <= 0 || nRasterYSize <= 0)
		throw std::runtime_error(format("Invalid dimensions: %d x %d", nRasterXSize, nRasterYSize));

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

	auto section = apiWrapper.getParams().section;
	if (section != Section::Unspecified)
		SetBand(1, new RasterBand(this, 1, static_cast<int>(section)));
}

GDALDataType Dataset::getSectionDataType(int sectionIndex)
{
	const auto aircomType = getPredRaster()->GetSectionDataType(sectionIndex);

	switch (aircomType)
	{
	case RasterSectionType_UnsignedChar:
		return GDT_Byte;
	case RasterSectionType_Short:
		return GDT_Int16;
	case RasterSectionType_UnsignedShort:
		return GDT_UInt16;
	case RasterSectionType_Int:
		return GDT_Int32;
	case RasterSectionType_UnsignedInt:
		return GDT_UInt32;
	case RasterSectionType_Float:
		return GDT_Float32;
	case RasterSectionType_Double:
		return GDT_Float64;
	default:
		throw std::runtime_error(format("Aircom data type %d not supported by GDAL", aircomType));
	}
}

void Dataset::setBoundingBox()
{
	const auto& bounds = getBoundingBox();
	nRasterXSize = width(bounds);
	nRasterYSize = height(bounds);

	double transformMatrix[6];
	transformMatrix[0] = bounds.min_corner().get<0>(); // minX
	transformMatrix[1] = getResolution();
	transformMatrix[2] = 0;
	// top-down
	transformMatrix[3] = bounds.max_corner().get<1>(); // maxY
	transformMatrix[4] = 0;
	transformMatrix[5] = -getResolution();

	SetGeoTransform(transformMatrix);
}

}}
