#include <gmock/gmock.h>

#include "ClutterCodes.h"

namespace aircom_map {

class ClutterCodesTests : public ::testing::Test
{
	std::stringstream menu;
	std::vector<std::string> clutterCodes;
	std::unique_ptr<ClutterCodes> indexClutterCodes;

public:
	void addClutter(std::string clutter)
	{
		menu << clutter << "\n";
	}

	void buildClutterCodes()
	{
		if (indexClutterCodes)
			return;

		indexClutterCodes = std::make_unique<ClutterCodes>(menu);
		
		const auto nrCodes = indexClutterCodes->getNrOfClutterCodes();

		clutterCodes.reserve(nrCodes);

		for (size_t i = 0; i < nrCodes; ++i)
			clutterCodes.push_back(indexClutterCodes->getClutterCodes()[i]);
	}

	std::vector<std::string>& getClutterCodes()
	{
		if (!indexClutterCodes)
			buildClutterCodes();

		return clutterCodes;
	}

	std::string& getClutterCode(int index)
	{
		if (!indexClutterCodes)
			buildClutterCodes();

		return clutterCodes[index];
	}

	std::string getGdalClutterCode(int index)
	{
		if(!indexClutterCodes)
			buildClutterCodes();

		return indexClutterCodes->getClutterCodes()[index];
	}
};

TEST_F(ClutterCodesTests, initializesWithEmptyList)
{
	EXPECT_EQ(0, getClutterCodes().size());
}

TEST_F(ClutterCodesTests, throwsIfIsNotInteger)
{
	addClutter("one water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(ClutterCodesTests, readsSingleClutter)
{
	addClutter("1 water");

	EXPECT_EQ("water", getClutterCode(1));
}

TEST_F(ClutterCodesTests, readsMultipleClutter)
{
	addClutter("0 water");
	addClutter("1 land");

	EXPECT_EQ("water", getClutterCode(0));
	EXPECT_EQ("land", getClutterCode(1));
}

TEST_F(ClutterCodesTests, readsMultipleWordsClutter)
{
	addClutter("0 supports multiple words");

	EXPECT_EQ("supports multiple words", getClutterCode(0));
}

TEST_F(ClutterCodesTests, handlesUndefinedClutterValues)
{
	addClutter("0 water");
	addClutter("2 land");

	EXPECT_EQ("", getClutterCode(1));
}

TEST_F(ClutterCodesTests, readsNumbersInClutterName)
{
	addClutter("0 supports 1 numbers 20 in 303 name 4567");

	EXPECT_EQ("supports 1 numbers 20 in 303 name 4567", getClutterCode(0));
}

TEST_F(ClutterCodesTests, supportsTabAndSpaceSeperatedIndex)
{
	addClutter("0\t\twater");
	addClutter("1    land");
	addClutter("2 \t mountain");

	EXPECT_EQ("water", getClutterCode(0));
	EXPECT_EQ("land", getClutterCode(1));
	EXPECT_EQ("mountain", getClutterCode(2));
}

TEST_F(ClutterCodesTests, supportsTabAndSpaceSeperatedName)
{
	addClutter("0\t supports  \t\t   tab    and\t\tspace seperated \t  \t   words");

	EXPECT_EQ("supports tab and space seperated words", getClutterCode(0));
}

TEST_F(ClutterCodesTests, throwsIfIsNegative)
{
	addClutter("-1 water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(ClutterCodesTests, throwsIfTooBig)
{
	addClutter("32768 water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(ClutterCodesTests, throwsIfNoOrNameIsDefined)
{
	addClutter("0");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(ClutterCodesTests, supportsGDALClutterCodes)
{
	addClutter("0 water");
	addClutter("2 mountain");

	EXPECT_EQ("water", getGdalClutterCode(0));
	EXPECT_EQ("", getGdalClutterCode(1));
	EXPECT_EQ("mountain", getGdalClutterCode(2));
}

}
