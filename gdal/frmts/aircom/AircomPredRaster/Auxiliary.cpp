#include "Auxiliary.h"

#include "StringUtils.h"

#include <boost/lexical_cast.hpp>

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
		parsePoint(sectionNode.get_child(L"BottomLeft")),
		parsePoint(sectionNode.get_child(L"TopRight")),
	};
}

std::map<unsigned long, GDALDataType> parseSectionDataTypes(const wptree& sectionDataTypesNode)
{
	static const std::map<std::wstring, GDALDataType> dataTypeMap = {
		{ L"U8", GDT_Byte},
		{ L"I16", GDT_Int16},
		{ L"U16", GDT_UInt16},
		{ L"I32", GDT_Int32},
		{ L"U32", GDT_UInt32},
		{ L"R32", GDT_Float32},
		{ L"R64", GDT_Float64}
	};

	std::map<unsigned long, GDALDataType> ret;
	for (const auto& kv : sectionDataTypesNode)
	{
		auto string = kv.second.get_value<std::wstring>();
		auto mapLookup = dataTypeMap.find(string);
		if (mapLookup == dataTypeMap.end())
			throw std::runtime_error(format("Unsupported section data type %s", string));

		ret.emplace(boost::lexical_cast<unsigned long>(kv.first), mapLookup->second);
	}

	return ret;
}

}

Auxiliary::Auxiliary(const MapBox& boundingBox, std::map<unsigned long, GDALDataType> sectionDataTypes, MapPoint tileSizeInPixels)
	: boundingBox(boundingBox)
	, sectionDataTypes(std::move(sectionDataTypes))
	, tileSizeInPixels(tileSizeInPixels)
{}

Auxiliary::Auxiliary(const wptree& auxiliaryNode)
	: boundingBox(parseBoundingBox(auxiliaryNode.get_child(L"BoundingBox")))
	, sectionDataTypes(parseSectionDataTypes(auxiliaryNode.get_child(L"SectionDataTypes")))
	, tileSizeInPixels(parsePoint(auxiliaryNode.get_child(L"TileSizeInPixels")))
{}

bool Auxiliary::operator==(const Auxiliary& r) const
{
	return boundingBox == r.boundingBox
		&& sectionDataTypes == r.sectionDataTypes
		&& tileSizeInPixels == r.tileSizeInPixels;
}

std::ostream& operator<<(std::ostream& stream, const std::map<unsigned long, GDALDataType> sectionDataTypes)
{
	stream << "{ ";
	bool first = true;
	for (const auto& kv : sectionDataTypes)
	{
		stream << kv.first << ": " << kv.second;
		if (!first)
			stream << ", ";
		first = false;
	}
	stream << " }";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Auxiliary& auxiliary)
{
	stream << "{" 
		<< auxiliary.boundingBox
		<< ", dataTypes: " << auxiliary.sectionDataTypes
		<< ", tile size: " << auxiliary.tileSizeInPixels
		<< "}";

	return stream;
}



}}
