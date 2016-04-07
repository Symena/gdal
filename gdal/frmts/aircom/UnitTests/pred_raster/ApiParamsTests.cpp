#include "AircomPredRaster/ApiParams.h"

#include <gmock/gmock.h>

#include <boost/property_tree/ptree.hpp>
#include <sstream>

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
	ApiParams apiParams(apiNode);
	EXPECT_EQ(Section::Unspecified, apiParams.section);
}

TEST_F(ApiParamsTests, ExceptionOnInvalidSection)
{
	apiNode.add(L"Section", L"invalid");
	EXPECT_THROW(ApiParams{apiNode}, std::runtime_error);
}

TEST_F(ApiParamsTests, WithSpecifiedSection)
{
	apiNode.add(L"Section", L"pathloss");
	ApiParams apiParams1(apiNode);

	EXPECT_EQ(Section::PathlossOnly, apiParams1.section);

	apiNode.put(L"Section", L"inclination");
	ApiParams apiParams2(apiNode);

	EXPECT_EQ(Section::InclinationOnly, apiParams2.section);
}

TEST_F(ApiParamsTests, ParamsParsedCorrectly)
{
	ApiParams apiParams(apiNode);

	EXPECT_EQ(boost::filesystem::path(L"c:/pred"), apiParams.predictionFolder);
	EXPECT_EQ(L"123-456-789", apiParams.predAccessClassID);
	EXPECT_EQ(L"123-456", apiParams.predRasterClassID);

	PredData expectedPredData;
	expectedPredData.nX_cm = 1;
	expectedPredData.nY_cm = 2;
	expectedPredData.nAntennaHeight_cm = 3;
	expectedPredData.nGroundHeight_cm = 4;
	expectedPredData.nResolution_cm = 5;
	expectedPredData.nRadius_cm = 6;
	expectedPredData.fFrequency_MHz = 7.1f;
	expectedPredData.nModelCRC = 8;
	expectedPredData.nPredFlags = 9;
	expectedPredData.nAntennaCRC = 10;
	expectedPredData.fAntennaMechanicalTilt_deg = 11.1;
	expectedPredData.fAntennaAzimuth_deg = 12.1;
	expectedPredData.nCwWeight = 13;
	expectedPredData.fCwRolloff = 14.1f;

	EXPECT_TRUE( 0 == std::memcmp( &expectedPredData, &apiParams.predData, sizeof(PredData) ) ) << "PredData mismatch";
}

}}
