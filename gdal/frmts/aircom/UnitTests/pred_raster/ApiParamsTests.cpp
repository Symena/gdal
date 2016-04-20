#include "AircomPredRaster/ApiParams.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

using boost::property_tree::wptree;

struct ApiParamsTests : public testing::Test
{
	wptree apiNode;
	wptree predDataNode;

	ApiParamsTests()
	{
		apiNode.add(L"PredictionFolder", L"c:/pred");
		apiNode.add(L"PredAccessClassID", L"123-456-789");
		apiNode.add(L"PredRasterClassID", L"123-456");

		predDataNode.add<std::int64_t>(L"nX_cm", 1);
		predDataNode.add<std::int64_t>(L"nY_cm", 2);

		predDataNode.add<std::int32_t>(L"nAntennaHeight_cm", 3);
		predDataNode.add<std::int32_t>(L"nGroundHeight_cm", 4);
		predDataNode.add<std::uint32_t>(L"nResolution_cm", 5);
		predDataNode.add<std::uint32_t>(L"nRadius_cm", 6);

		predDataNode.add<float>(L"fFrequency_MHz", 7.1f);
		predDataNode.add<std::uint64_t>(L"nModelCRC", 8);
		predDataNode.add<std::uint32_t>(L"nPredFlags", 9);
		predDataNode.add<std::uint64_t>(L"nAntennaCRC", 10);

		predDataNode.add<double>(L"fAntennaMechanicalTilt_deg", 11.1);
		predDataNode.add<double>(L"fAntennaAzimuth_deg", 12.1);
		predDataNode.add<std::uint16_t>(L"nCwWeight", 13);
		predDataNode.add<float>(L"fCwRolloff", 14.1f);

		apiNode.add_child(L"PredData", predDataNode);
	}
};

TEST_F(ApiParamsTests, NoSectionSpecified)
{	
	ApiParams apiParams(apiNode, {});
	EXPECT_EQ(Section::Unspecified, apiParams.section);
}

TEST_F(ApiParamsTests, ExceptionOnInvalidSection)
{
	apiNode.add(L"Section", L"invalid");
	EXPECT_THROW(ApiParams(apiNode, {}), std::runtime_error);
}

TEST_F(ApiParamsTests, WithSpecifiedSection)
{
	apiNode.add(L"Section", L"pathloss");
	ApiParams apiParams1(apiNode, {});

	EXPECT_EQ(Section::PathlossOnly, apiParams1.section);

	apiNode.put(L"Section", L"inclination");
	ApiParams apiParams2(apiNode, {});

	EXPECT_EQ(Section::InclinationOnly, apiParams2.section);
}

TEST_F(ApiParamsTests, ParamsParsedCorrectly)
{
	ApiParams apiParams(apiNode, {});

	EXPECT_EQ(boost::filesystem::path(L"c:/pred"), apiParams.predictionFolder);
	EXPECT_EQ(L"123-456-789", apiParams.predAccessClassID);
	EXPECT_EQ(L"123-456", apiParams.predRasterClassID);

	const auto& pd = apiParams.predData;

	EXPECT_EQ( 1,    pd.nX_cm);
	EXPECT_EQ( 2,    pd.nY_cm);
	EXPECT_EQ( 3,    pd.nAntennaHeight_cm);
	EXPECT_EQ( 4,    pd.nGroundHeight_cm);
	EXPECT_EQ( 5,    pd.nResolution_cm);
	EXPECT_EQ( 6,    pd.nRadius_cm);
	EXPECT_EQ( 7.1f, pd.fFrequency_MHz);
	EXPECT_EQ( 8,    pd.nModelCRC);
	EXPECT_EQ( 9,    pd.nPredFlags);
	EXPECT_EQ(10,    pd.nAntennaCRC);
	EXPECT_EQ(11.1,  pd.fAntennaMechanicalTilt_deg);
	EXPECT_EQ(12.1,  pd.fAntennaAzimuth_deg);
	EXPECT_EQ(13,    pd.nCwWeight);
	EXPECT_EQ(14.1f, pd.fCwRolloff);
}

TEST_F(ApiParamsTests, predictionFolder_relativeToGapFile)
{
	apiNode.get_child(L"PredictionFolder").put_value(L"../../n/e");
	ApiParams apiParams(apiNode, "z:/o/m/b/i.gap");

	EXPECT_EQ(boost::filesystem::path(L"z:/o/m/b/../../n/e"), apiParams.predictionFolder);
}

}}
