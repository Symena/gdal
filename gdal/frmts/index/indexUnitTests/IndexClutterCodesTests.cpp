#include <gmock/gmock.h>

#include "IndexClutterCodes.h"

class IndexClutterCodesTests : public ::testing::Test
{
	std::stringstream menu;
	std::vector<std::string> clutterCodes;
	std::unique_ptr<IndexClutterCodes> indexClutterCodes;

public:
	void addClutter(std::string clutter)
	{
		menu << clutter << "\n";
	}

	void buildClutterCodes()
	{
		if (indexClutterCodes)
			return;

		indexClutterCodes = std::make_unique<IndexClutterCodes>(menu);
		clutterCodes = indexClutterCodes->getClutterCodes();
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
};

TEST_F(IndexClutterCodesTests, initializesWithEmptyList)
{
	EXPECT_EQ(0, getClutterCodes().size());
}

TEST_F(IndexClutterCodesTests, throwsIfIndexIsNotInteger)
{
	addClutter("one water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(IndexClutterCodesTests, readsSingleClutter)
{
	addClutter("1 water");

	EXPECT_EQ("water", getClutterCode(1));
}

TEST_F(IndexClutterCodesTests, readsMultipleClutter)
{
	addClutter("0 water");
	addClutter("1 land");

	EXPECT_EQ("water", getClutterCode(0));
	EXPECT_EQ("land", getClutterCode(1));
}

TEST_F(IndexClutterCodesTests, readsMultipleWordsClutter)
{
	addClutter("0 supports multiple words");

	EXPECT_EQ("supports multiple words", getClutterCode(0));
}

TEST_F(IndexClutterCodesTests, handlesUndefinedClutterValues)
{
	addClutter("0 water");
	addClutter("2 land");

	EXPECT_EQ("", getClutterCode(1));
}

TEST_F(IndexClutterCodesTests, readsNumbersInClutterName)
{
	addClutter("0 supports 1 numbers 20 in 303 name 4567");

	EXPECT_EQ("supports 1 numbers 20 in 303 name 4567", getClutterCode(0));
}

TEST_F(IndexClutterCodesTests, supportsTabAndSpaceSeperatedIndex)
{
	addClutter("0\t\twater");
	addClutter("1    land");
	addClutter("2 \t mountain");

	EXPECT_EQ("water", getClutterCode(0));
	EXPECT_EQ("land", getClutterCode(1));
	EXPECT_EQ("mountain", getClutterCode(2));
}

TEST_F(IndexClutterCodesTests, supportsTabAndSpaceSeperatedName)
{
	addClutter("0\t supports  \t\t   tab    and\t\tspace seperated \t  \t   words");

	EXPECT_EQ("supports tab and space seperated words", getClutterCode(0));
}

TEST_F(IndexClutterCodesTests, throwsIfIndexIsNegative)
{
	addClutter("-1 water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(IndexClutterCodesTests, throwsIfIndexTooBig)
{
	addClutter("32768 water");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}

TEST_F(IndexClutterCodesTests, throwsIfNoIndexOrNameIsDefined)
{
	addClutter("0");

	EXPECT_THROW(buildClutterCodes(), std::runtime_error);
}
