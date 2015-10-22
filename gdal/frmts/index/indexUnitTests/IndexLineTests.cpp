#include <gmock/gmock.h>

#include "IndexLine.h"

class IndexLineTests : public ::testing::Test
{
public:

	IndexWarnings warnings;

	IndexLine getLine(const std::string& line)
	{
		return IndexLine(line, warnings);
	}

	size_t warningCount() const { return std::distance(warnings.begin(), warnings.end()); }
};

TEST_F(IndexLineTests, parsesFilenameWithoutExtension)
{
	auto line = getLine("file 0 0 0 0 1");

	EXPECT_EQ("file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesFilenameWithExtension)
{
	auto line = getLine("file.ext 0 0 0 0 1");

	EXPECT_EQ("file.ext", line.getTilePath());
}

TEST_F(IndexLineTests, parsesRelativePathBackslash)
{
	auto line = getLine(R"(path\to\file 0 0 0 0 1)");

	EXPECT_EQ("path/to/file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesRelativePathForwardSlash)
{
	auto line = getLine(R"(path/to/file 0 0 0 0 1)");

	EXPECT_EQ("path/to/file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesWithTabsAndMultipleSpacesAsSeparators)
{
	EXPECT_NO_THROW(getLine("file       0 \t 0  0\t0\t\t1"));
}

TEST_F(IndexLineTests, parsesWithCarriageReturnAtLineEnd)
{
	EXPECT_NO_THROW(getLine("file 0 0 0 0 1\r"));
}

TEST_F(IndexLineTests, failsOnTooFewTokens)
{
	EXPECT_THROW(getLine("file 1 1 1 1"), std::runtime_error);
}

TEST_F(IndexLineTests, failsOnTooManyTokens)
{
	EXPECT_THROW(getLine("file 1 1 1 1 1 1"), std::runtime_error);
}

TEST_F(IndexLineTests, wrongNumberOfTokensExceptionContainsFoundNumberOfTokens)
{
	try { getLine("1 2 3 4 5 6 7 8 9 10 11"); }
	catch(const std::runtime_error& e)
	{
		EXPECT_EQ("Found 11 tokens, expected 6", std::string(e.what()));
	}
}

TEST_F(IndexLineTests, readsAllValues)
{
	const auto& line = getLine("file 5 6 3 4 1");

	EXPECT_EQ("file", line.getTilePath());
	EXPECT_EQ(5, line.getTileEastMin());
	EXPECT_EQ(6, line.getTileEastMax());
	EXPECT_EQ(3, line.getTileNorthMin());
	EXPECT_EQ(4, line.getTileNorthMax());
	EXPECT_EQ(1, line.getPixelSquareSize());
}

TEST_F(IndexLineTests, readsNegativeCoordinateValues)
{
	const auto& line = getLine("file -5 6 3 4 1");

	EXPECT_EQ(-5, line.getTileEastMin());
}

TEST_F(IndexLineTests, failsOnFloatCoordinates)
{
	EXPECT_THROW(getLine("file 5.0 6 3 4 1"), std::runtime_error);
}

TEST_F(IndexLineTests, warnsForEastingValuesNotMultipleOfPixelSize)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 5").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(IndexLineTests, warnsForNorthingValuesNotMultipleOfPixelSize)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 4").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(IndexLineTests, warnsForBothNorthingAndEastingErrors)
{
	EXPECT_FALSE(getLine("file 3 7 4 8 5").isConsistent());
	EXPECT_EQ(2, warningCount());
}

TEST_F(IndexLineTests, warnsForNegativePixelSizes)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 -1").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(IndexLineTests, warnsForZeroPixelSizes)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 0").isConsistent());
	EXPECT_EQ(1, warningCount());
}