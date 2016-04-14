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

const std::map<std::wstring, GDALDataType> dataTypeMap = {
		{ L"U8", GDT_Byte},
		{ L"I16", GDT_Int16},
		{ L"U16", GDT_UInt16},
		{ L"I32", GDT_Int32},
		{ L"U32", GDT_UInt32},
		{ L"R32", GDT_Float32},
		{ L"R64", GDT_Float64}
	};

std::map<unsigned long, GDALDataType> parseSectionDataTypes(const wptree& sectionDataTypesNode)
{
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

void addMapPointChild(wptree& tree, const std::wstring& path, const MapPoint& mapPoint)
{
	wptree x, y, values;
	x.put(L"", mapPoint.get<0>());
	y.put(L"", mapPoint.get<1>());

	values.push_back(std::make_pair(L"", x));
	values.push_back(std::make_pair(L"", y));

	tree.add_child(path, values);
}

}

Auxiliary::Auxiliary(const MapBox& boundingBox, int epsg, std::map<unsigned long, GDALDataType> sectionDataTypes, MapPoint tileSizeInPixels)
	: boundingBox(boundingBox)
	, epsg(epsg)
	, sectionDataTypes(std::move(sectionDataTypes))
	, tileSizeInPixels(tileSizeInPixels)
{}

Auxiliary::Auxiliary(const wptree& auxiliaryNode)
	: boundingBox(parseBoundingBox(auxiliaryNode.get_child(L"BoundingBox")))
	, epsg(auxiliaryNode.get<int>(L"EPSG"))
	, sectionDataTypes(parseSectionDataTypes(auxiliaryNode.get_child(L"SectionDataTypes")))
	, tileSizeInPixels(parsePoint(auxiliaryNode.get_child(L"TileSizeInPixels")))
{}

boost::property_tree::wptree Auxiliary::asPropertyTree() const
{
	wptree auxiliary, ptBoundingBox, ptSectionDatatTypes;
	addMapPointChild(ptBoundingBox, L"BottomLeft", boundingBox.min_corner());
	addMapPointChild(ptBoundingBox, L"TopRight", boundingBox.max_corner());

	auxiliary.add_child(L"BoundingBox", ptBoundingBox);
	auxiliary.add(L"EPSG", epsg);
	addMapPointChild(auxiliary, L"TileSizeInPixels", tileSizeInPixels);
	
	for (const auto& sectionPair : sectionDataTypes)
	{
		std::wstring stringDataType;
		for (const auto& kv : dataTypeMap)
			if (kv.second == sectionPair.second)
			{
				stringDataType = kv.first;
				break;
			}

		if (stringDataType.empty())
			throw std::runtime_error(format("No string representation found for GDALDataType %d", sectionPair.second));

		ptSectionDatatTypes.add(std::to_wstring(sectionPair.first), stringDataType);
	}
	
	auxiliary.add_child(L"SectionDataTypes", ptSectionDatatTypes);

	return auxiliary;
}

bool Auxiliary::operator==(const Auxiliary& r) const
{
	return boundingBox == r.boundingBox
		&& epsg == r.epsg
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
		<< ", EPSG: " << auxiliary.epsg
		<< ", dataTypes: " << auxiliary.sectionDataTypes
		<< ", tile size: " << auxiliary.tileSizeInPixels
		<< "}";

	return stream;
}



}}
