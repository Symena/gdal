#include "ApiWrapper.h"

#include "PredRasterFactory.h"
#include "StringUtils.h"

#include <mutex>
#include <unordered_map>

namespace aircom { namespace pred_raster {

namespace {

MapBox getSectionArea(IPredRaster5Ptr predRaster, unsigned long sectionNum, int& epsg)
{
	_REGIONEX region;
	predRaster->GetRegionEx(sectionNum, &region);

	const MapPoint topLeft(int(region.m_eastMin / 100), int(region.m_northMax / 100));

	const int res = region.m_resolution / 100;
	const MapPoint bottomLeft(topLeft.get<0>(), topLeft.get<1>() - region.m_height * res);
	const MapPoint topRight(topLeft.get<0>() + region.m_width * res, topLeft.get<1>());

	epsg = region.m_EPSG;
	return { bottomLeft, topRight };
}

std::mutex coInitialize_mutex;
std::unordered_map<std::thread::id, bool> coInitialized;

}

ApiWrapper::ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster)
	: params(std::move(apiParams)) 
	, predRaster(predRaster)
{
	auto threadId = std::this_thread::get_id();
	std::lock_guard<std::mutex> lock(coInitialize_mutex);
	if (!coInitialized[threadId])
	{
		CoInitialize(nullptr);
		coInitialized[threadId] = true;
	}
}

IPredRaster5Ptr ApiWrapper::getPredRaster()
{
	if (!predRaster)
		predRaster = PredRasterFactory::createPredRaster(params);

	return predRaster;
}

std::vector<unsigned long> ApiWrapper::getSectionNums()
{
	auto numSections = getPredRaster()->GetNumSections();

	std::vector<unsigned long> sections(numSections);
	getPredRaster()->GetSectionNumList(numSections, sections.data());

	return sections;
}

GDALDataType ApiWrapper::getDataType(unsigned long sectionNum)
{
	const auto aircomType = getPredRaster()->GetSectionDataType(sectionNum);

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

MapPoint ApiWrapper::getTileSizeInPixels(unsigned long sectionNum)
{
	auto firstTile = getPredRaster()->CreateTileIterator(sectionNum)->GetNextTile();

	_REGIONEX tileRegion;
	firstTile->GetTileRegion(&tileRegion);

	return { tileRegion.m_width, tileRegion.m_height };
}

SectionInfos ApiWrapper::getSectionInfos()
{
	SectionInfos result;
	for (auto sectionNum : getSectionNums())
	{
		SectionInfo sectionInfo = { getDataType(sectionNum), getTileSizeInPixels(sectionNum) };
		result.emplace(sectionNum, sectionInfo);
	}

	return result;
}

Auxiliary ApiWrapper::getAuxiliary()
{
	auto sectionInfos = getSectionInfos();
	auto firstSectionNum = sectionInfos.begin()->first;

	int epsg;
	auto bounds = getSectionArea(getPredRaster(), firstSectionNum, epsg);

	return Auxiliary(bounds, epsg, std::move(sectionInfos));
}

}}
