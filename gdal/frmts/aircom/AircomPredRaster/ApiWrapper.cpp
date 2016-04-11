#include "ApiWrapper.h"

#include "PredRasterFactory.h"
#include "StringUtils.h"

#include <mutex>

namespace aircom { namespace pred_raster {

namespace {

GDALDataType getDataType(IPredRaster5Ptr predRaster, unsigned long sectionNum)
{
	const auto aircomType = predRaster->GetSectionDataType(sectionNum);

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

MapPoint getTileSizeInPixels(IPredRaster5Ptr predRaster, unsigned long sectionNum)
{
	auto firstTile = predRaster->CreateTileIterator(sectionNum)->GetNextTile();

	_REGIONEX tileRegion;
	firstTile->GetTileRegion(&tileRegion);

	return { tileRegion.m_width, tileRegion.m_height };
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

SectionInfo ApiWrapper::getSectionInfo(unsigned long sectionNum)
{
	auto predRaster = getPredRaster();

	_REGIONEX region;
	predRaster->GetRegionEx(0, &region);

	const MapPoint topLeft(int(region.m_eastMin / 100), int(region.m_northMax / 100));

	const int res = region.m_resolution / 100;
	const MapPoint bottomLeft(topLeft.get<0>(), topLeft.get<1>() - region.m_height * res);
	const MapPoint topRight(topLeft.get<0>() + region.m_width * res, topLeft.get<1>());

	return SectionInfo(
		{ bottomLeft, topRight },
		getDataType(predRaster, sectionNum),
		getTileSizeInPixels(predRaster, sectionNum)
	);
}

}}
