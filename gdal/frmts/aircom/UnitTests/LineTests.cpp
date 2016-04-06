#include <gmock/gmock.h>

#include "Line.h"

namespace aircom_map {

class LineTests : public ::testing::Test
{
public:

	Warnings warnings;

	Line getLine(const std::string& line, const std::string& dataRootDir = {})
	{
		return Line(line, warnings, dataRootDir);
	}

	size_t warningCount() const { return std::distance(warnings.begin(), warnings.end()); }
};

TEST_F(LineTests, parsesFilenameWithoutExtension)
{
	auto line = getLine("file 0 0 0 0 1");

	EXPECT_EQ("file", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesFilenameWithExtension)
{
	auto line = getLine("file.ext 0 0 0 0 1");

	EXPECT_EQ("file.ext", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesRelativePathBackslash)
{
	auto line = getLine(R"(path\to\file 0 0 0 0 1)");

	EXPECT_EQ("path\\to\\file", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesRelativePathForwardSlash)
{
	auto line = getLine(R"(path/to/file 0 0 0 0 1)");

	EXPECT_EQ("path/to/file", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesPathToParent)
{
	auto line = getLine(R"(../to/file 0 0 0 0 1)", "parent/path");

	// With boost 1.60 could be changed to "parent/to/file"
	// see: https://svn.boost.org/trac/boost/ticket/5897
	EXPECT_EQ(R"(parent/path\../to/file)", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesAbsolutePath)
{
	auto line = getLine(R"(a:\path\to\file 0 0 0 0 1)", "parent/path");

	EXPECT_EQ(R"(a:\path\to\file)", line.getTileDataSource()->getStreamDescription());
}

TEST_F(LineTests, parsesWithTabsAndMultipleSpacesAsSeparators)
{
	EXPECT_NO_THROW(getLine("file       0 \t 0  0\t0\t\t1"));
}

TEST_F(LineTests, parsesWithCarriageReturnAtLineEnd)
{
	EXPECT_NO_THROW(getLine("file 0 0 0 0 1\r"));
}

TEST_F(LineTests, failsOnTooFewTokens)
{
	EXPECT_THROW(getLine("file 1 1 1 1"), std::runtime_error);
}

TEST_F(LineTests, failsOnTooManyTokens)
{
	EXPECT_THROW(getLine("file 1 1 1 1 1 1"), std::runtime_error);
}

TEST_F(LineTests, wrongNumberOfTokensExceptionContainsFoundNumberOfTokens)
{
	try { getLine("1 2 3 4 5 6 7 8 9 10 11"); }
	catch(const std::runtime_error& e)
	{
		EXPECT_EQ("Found 11 tokens, expected 6", std::string(e.what()));
	}
}

TEST_F(LineTests, readsAllValues)
{
	const auto& line = getLine("file 5 6 3 4 1");

	EXPECT_EQ("file", line.getTileDataSource()->getStreamDescription());
	EXPECT_EQ(5, line.getTileEastMin());
	EXPECT_EQ(6, line.getTileEastMax());
	EXPECT_EQ(3, line.getTileNorthMin());
	EXPECT_EQ(4, line.getTileNorthMax());
	EXPECT_EQ(1, line.getResolution());
}

TEST_F(LineTests, readsNegativeCoordinateValues)
{
	const auto& line = getLine("file -5 6 3 4 1");

	EXPECT_EQ(-5, line.getTileEastMin());
}

TEST_F(LineTests, failsOnFloatCoordinates)
{
	EXPECT_THROW(getLine("file 5.0 6 3 4 1"), std::runtime_error);
}

TEST_F(LineTests, warnsForEastingValuesNotMultipleOfPixelSize)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 5").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(LineTests, warnsForNorthingValuesNotMultipleOfPixelSize)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 4").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(LineTests, warnsForBothNorthingAndEastingErrors)
{
	EXPECT_FALSE(getLine("file 3 7 4 8 5").isConsistent());
	EXPECT_EQ(2, warningCount());
}

TEST_F(LineTests, warnsForNegativePixelSizes)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 -1").isConsistent());
	EXPECT_EQ(1, warningCount());
}

TEST_F(LineTests, warnsForZeroPixelSizes)
{
	EXPECT_FALSE(getLine("file 3 7 4 9 0").isConsistent());
	EXPECT_EQ(1, warningCount());
}

}
