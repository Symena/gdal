#include "SectionInfo.h"

#include "StringUtils.h"

using boost::property_tree::wptree;

namespace aircom { namespace pred_raster {

namespace {

MapPoint parsePoint(const wptree& pointNode)
{
	if (pointNode.size() != 2)
		throw std::runtime_error("Failed to parse a 2D point - 2 coordinates expected");

	auto it = pointNode.begin();
	int x = (it++)->second.get_value<int>();
	int y = it->second.get_value<int>();

	return { x, y };
}

MapBox parseBoundingBox(const wptree& sectionNode)
{
	return {
		parsePoint(sectionNode.get_child(L"bottomLeft")),
		parsePoint(sectionNode.get_child(L"topRight")),
	};
}

GDALDataType parseDataType(const wptree& sectionNode)
{
	const auto string = sectionNode.get<std::wstring>(L"dataType");

	if (string == L"U8")
		return GDT_Byte;
	if (string == L"I16")
		return GDT_Int16;
	if (string == L"U16")
		return GDT_UInt16;
	if (string == L"I32")
		return GDT_Int32;
	if (string == L"U32")
		return GDT_UInt32;
	if (string == L"R32")
		return GDT_Float32;
	if (string == L"R64")
		return GDT_Float64;

	throw std::runtime_error(format("Unsupported section data type %s", string));
}

}

SectionInfo::SectionInfo(const MapBox& boundingBox, GDALDataType dataType, MapPoint tileSizeInPixels, MapPoint numTiles)
	: boundingBox(boundingBox)
	, dataType(dataType)
	, tileSizeInPixels(tileSizeInPixels)
	, numTiles(numTiles)
{}

SectionInfo::SectionInfo(const wptree& sectionNode)
	: boundingBox(parseBoundingBox(sectionNode))
	, dataType(parseDataType(sectionNode))
	, tileSizeInPixels(parsePoint(sectionNode.get_child(L"tileSizeInPixels")))
	, numTiles(parsePoint(sectionNode.get_child(L"numTiles")))
{}

bool SectionInfo::operator==(const SectionInfo& r) const
{
	return boundingBox == r.boundingBox
		&& dataType == r.dataType
		&& tileSizeInPixels == r.tileSizeInPixels
		&& numTiles == r.numTiles;
}

std::ostream& operator<<(std::ostream& stream, const SectionInfo& sectionInfo)
{
	stream << "{" 
		<< sectionInfo.boundingBox
		<< ", data type: " << sectionInfo.dataType
		<< ", tile size: " << sectionInfo.tileSizeInPixels
		<< ", num tiles: " << sectionInfo.numTiles
		<< "}";

	return stream;
}

}}
