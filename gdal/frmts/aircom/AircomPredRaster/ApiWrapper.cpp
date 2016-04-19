#include "ApiWrapper.h"

#include "PredRasterFactory.h"
#include "StringUtils.h"

namespace aircom { namespace pred_raster {

ApiWrapper::ApiWrapper(ApiParams apiParams, IPredRaster5Ptr predRaster)
	: params(std::move(apiParams)) 
	, predRaster(std::move(predRaster))
{}

IPredRaster5Ptr ApiWrapper::getPredRaster()
{
	if (!predRaster)
		predRaster = PredRasterFactory::createPredRaster(params);

	return predRaster;
}

std::vector<unsigned long> ApiWrapper::getSectionNums()
{
	const auto numSections = getPredRaster()->GetNumSections();

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

Point ApiWrapper::getTileSizeInPixels(unsigned long sectionNum)
{
	auto firstTile = getPredRaster()->CreateTileIterator(sectionNum)->GetNextTile();

	_REGIONEX tileRegion;
	firstTile->GetTileRegion(&tileRegion);

	return { tileRegion.m_width, tileRegion.m_height };
}

Rectangle ApiWrapper::getBounds(unsigned long sectionNum, int& epsg)
{
	_REGIONEX region;
	getPredRaster()->GetRegionEx(sectionNum, &region);

	const Point topLeft(int(region.m_eastMin / 100), int(region.m_northMax / 100));

	const int res = region.m_resolution / 100;
	const Point bottomLeft(topLeft.x(), topLeft.y() - region.m_height * res);
	const Point topRight(topLeft.x() + region.m_width * res, topLeft.y());

	epsg = region.m_EPSG;
	return { bottomLeft, topRight };
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

	const auto firstSectionNum = sectionInfos.begin()->first;
	int epsg;
	const auto bounds = getBounds(firstSectionNum, epsg);

	return Auxiliary(bounds, epsg, std::move(sectionInfos));
}

}}
