#include "ApiParams.h"

#include "StringUtils.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <cstdint>

using boost::property_tree::wptree;

namespace aircom { namespace pred_raster {

namespace {

PredData parsePredData(const wptree& predDataNode)
{
	PredData r;

	r.nX_cm = predDataNode.get<std::int64_t>(L"nX_cm");
	r.nY_cm = predDataNode.get<std::int64_t>(L"nY_cm");
	r.nAntennaHeight_cm = predDataNode.get<std::int32_t>(L"nAntennaHeight_cm");
	r.nGroundHeight_cm = predDataNode.get<std::int32_t>(L"nGroundHeight_cm");
	r.nResolution_cm = predDataNode.get<std::uint32_t>(L"nResolution_cm");
	r.nRadius_cm = predDataNode.get<std::uint32_t>(L"nRadius_cm");
	r.fFrequency_MHz = predDataNode.get<float>(L"fFrequency_MHz");
	r.nModelCRC = predDataNode.get<std::uint64_t>(L"nModelCRC");
	r.nPredFlags = predDataNode.get<std::uint32_t>(L"nPredFlags");
	r.nAntennaCRC = predDataNode.get<std::uint64_t>(L"nAntennaCRC");
	r.fAntennaMechanicalTilt_deg = predDataNode.get<double>(L"fAntennaMechanicalTilt_deg");
	r.fAntennaAzimuth_deg = predDataNode.get<double>(L"fAntennaAzimuth_deg");
	r.nCwWeight = predDataNode.get<std::uint16_t>(L"nCwWeight");
	r.fCwRolloff = predDataNode.get<float>(L"fCwRolloff");

	return r;
}

Section parseSection(const boost::optional<std::wstring>& optionalSection)
{
	if (!optionalSection)
		return Section::Unspecified;

	auto section = optionalSection.value();
	boost::to_lower(section);

	if (section == L"pathloss")
		return Section::PathlossOnly;
	if (section == L"inclination")
		return Section::InclinationOnly;

	throw std::runtime_error(ws2ns(format(L"Unknown section '%1%'", section)));
}

}

ApiParams::ApiParams(boost::filesystem::path predictionFolder, PredData predData, std::wstring predAccessClassID, 
					 std::wstring predRasterClassID, Section section)
	: predictionFolder(std::move(predictionFolder))
	, predData(std::move(predData))
	, predAccessClassID(std::move(predAccessClassID))
	, predRasterClassID(std::move(predRasterClassID))
	, section(section)
{}

ApiParams::ApiParams(const wptree& apiNode) : ApiParams
	( apiNode.get<std::wstring>(L"PredictionFolder")
	, parsePredData(apiNode.get_child(L"PredData"))
	, apiNode.get<std::wstring>(L"PredAccessClassID")
	, apiNode.get<std::wstring>(L"PredRasterClassID")
	, parseSection(apiNode.get_optional<std::wstring>(L"Section")))
{}

}}
