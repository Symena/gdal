#include "Auxiliary.h"

#include "StringUtils.h"

#include <boost/lexical_cast.hpp>

using boost::property_tree::wptree;

namespace aircom { namespace pred_raster {

namespace {

Point parsePoint(const wptree& pointNode)
{
	if (pointNode.size() != 2)
		throw std::runtime_error("Failed to parse a 2D point - 2 coordinates expected");

	auto it = pointNode.begin();
	int x = (it++)->second.get_value<int>();
	int y = it->second.get_value<int>();

	return { x, y };
}

Rectangle parseBoundingBox(const wptree& sectionNode)
{
	return {
		parsePoint(sectionNode.get_child(L"BottomLeft")),
		parsePoint(sectionNode.get_child(L"TopRight")),
	};
}

const std::map<std::wstring, GDALDataType> dataTypesMap = {
	{ L"U8",  GDT_Byte},
	{ L"I16", GDT_Int16},
	{ L"U16", GDT_UInt16},
	{ L"I32", GDT_Int32},
	{ L"U32", GDT_UInt32},
	{ L"R32", GDT_Float32},
	{ L"R64", GDT_Float64}
};

SectionInfos parseSectionInfos(const wptree& sectionsNode)
{
	SectionInfos result;
	for (const auto& kv : sectionsNode)
	{
		const auto sectionNum = boost::lexical_cast<unsigned long>(kv.first);
		const auto& sectionInfoNode = kv.second;

		const auto dataTypeString = sectionInfoNode.get<std::wstring>(L"DataType");
		const auto it = dataTypesMap.find(dataTypeString);
		if (it == dataTypesMap.end())
			throw std::runtime_error(format("Unsupported section data type %s", dataTypeString));

		SectionInfo sectionInfo = { it->second, parsePoint(sectionInfoNode.get_child(L"TileSizeInPixels")) };
		result.emplace(sectionNum, sectionInfo);
	}

	return result;
}

void addPoint(wptree& tree, const std::wstring& path, const Point& point)
{
	wptree x, y;
	x.put(L"", point.x());
	y.put(L"", point.y());

	auto& pointNode = tree.add_child(path, wptree());
	pointNode.push_back(std::make_pair(L"", x));
	pointNode.push_back(std::make_pair(L"", y));
}

}

Auxiliary::Auxiliary(const Rectangle& boundingBox, int epsg, SectionInfos sectionInfos)
	: boundingBox(boundingBox)
	, epsg(epsg)
	, sectionInfos(std::move(sectionInfos))
{}

Auxiliary::Auxiliary(const wptree& auxiliaryNode) : Auxiliary
	( parseBoundingBox(auxiliaryNode.get_child(L"BoundingBox"))
	, auxiliaryNode.get<int>(L"EPSG")
	, parseSectionInfos(auxiliaryNode.get_child(L"Sections")))
{}

wptree Auxiliary::asPropertyTree() const
{
	wptree auxiliaryNode;

	auto& boundingBoxNode = auxiliaryNode.add_child(L"BoundingBox", wptree());
	addPoint(boundingBoxNode, L"BottomLeft", boundingBox.min_corner());
	addPoint(boundingBoxNode, L"TopRight", boundingBox.max_corner());

	auxiliaryNode.add(L"EPSG", epsg);

	auto& sectionsNode = auxiliaryNode.add_child(L"Sections", wptree());
	for (const auto& sectionPair : sectionInfos)
	{
		const auto dataType = sectionPair.second.dataType;

		const auto it = std::find_if(dataTypesMap.begin(), dataTypesMap.end(),
			[=](auto& kv) { return kv.second == dataType; });
		if (it == dataTypesMap.end())
			throw std::runtime_error(format("No string representation found for GDALDataType %d", dataType));

		auto& sectionNode = sectionsNode.add_child(std::to_wstring(sectionPair.first), wptree());
		sectionNode.add(L"DataType", it->first);
		addPoint(sectionNode, L"TileSizeInPixels", sectionPair.second.tileSizeInPixels);
	}

	return auxiliaryNode;
}

bool Auxiliary::operator==(const Auxiliary& r) const
{
	return boundingBox == r.boundingBox
		&& epsg == r.epsg
		&& sectionInfos == r.sectionInfos;
}

std::ostream& operator<<(std::ostream& stream, const SectionInfos& sectionInfos)
{
	stream << "{ ";
	bool first = true;
	for (const auto& kv : sectionInfos)
	{
		stream << kv.first << ": { data type: " << kv.second.dataType << ", tile size: " << kv.second.tileSizeInPixels << " }";
		if (!first)
			stream << ", ";
		first = false;
	}
	stream << " }";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Auxiliary& auxiliary)
{
	stream << "{ "
		<< auxiliary.boundingBox
		<< ", EPSG: " << auxiliary.epsg
		<< ", sections: " << auxiliary.sectionInfos
		<< " }";

	return stream;
}

}}
