#include <gmock/gmock.h>

#include "IndexLine.h"

class IndexLineTests : public ::testing::Test
{
	std::stringstream stream;

public:

	IndexLine getParsedLine(const std::string& line)
	{
		setLine(line);
		return IndexLine(stream);
	}

	void invalidateStream()
	{
		stream.setstate(std::ios::badbit);
	}

private:
	void setLine(const std::string& line) { stream << line; }
};

TEST_F(IndexLineTests, parsesFilenameWithoutExtension)
{
	auto line = getParsedLine("file 0 0 0 0 1");

	EXPECT_EQ("file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesFilenameWithExtension)
{
	auto line = getParsedLine("file.ext 0 0 0 0 1");

	EXPECT_EQ("file.ext", line.getTilePath());
}

TEST_F(IndexLineTests, parsesRelativePathBackslash)
{
	auto line = getParsedLine(R"(path\to\file 0 0 0 0 1)");

	EXPECT_EQ("path/to/file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesRelativePathForwardSlash)
{
	auto line = getParsedLine(R"(path/to/file 0 0 0 0 1)");

	EXPECT_EQ("path/to/file", line.getTilePath());
}

TEST_F(IndexLineTests, parsesWithTabsAndMultipleSpacesAsSeparators)
{
	EXPECT_NO_THROW(getParsedLine("file       0 \t 0  0\t0\t\t1"));
}

TEST_F(IndexLineTests, failsOnTooFewTokens)
{
	EXPECT_THROW(getParsedLine("file 1 1 1 1"), std::runtime_error);
}

TEST_F(IndexLineTests, failsOnTooManyTokens)
{
	EXPECT_THROW(getParsedLine("file 1 1 1 1 1 1"), std::runtime_error);
}

TEST_F(IndexLineTests, wrongNumberOfTokensExceptionContainsFoundNumberOfTokens)
{
	try { getParsedLine("1 2 3 4 5 6 7 8 9 10 11"); }
	catch(const std::runtime_error& e)
	{
		EXPECT_EQ("Found 11 tokens, expected 6", std::string(e.what()));
	}
}

TEST_F(IndexLineTests, failsOnInvalidStream)
{
	invalidateStream();

	EXPECT_THROW(getParsedLine("file 1 1 1 1 1"), std::runtime_error);
}

TEST_F(IndexLineTests, readsAllValues)
{
	const auto& line = getParsedLine("file 5 6 3 4 1");

	EXPECT_EQ("file", line.getTilePath());
	EXPECT_EQ(5, line.getTileEastMin());
	EXPECT_EQ(6, line.getTileEastMax());
	EXPECT_EQ(3, line.getTileNorthMin());
	EXPECT_EQ(4, line.getTileNorthMax());
	EXPECT_EQ(1, line.getPixelSquareSize());
}

TEST_F(IndexLineTests, readsNegativeCoordinateValues)
{
	const auto& line = getParsedLine("file -5 6 3 4 1");

	EXPECT_EQ(-5, line.getTileEastMin());
}

TEST_F(IndexLineTests, failsOnFloatCoordinates)
{
	EXPECT_THROW(getParsedLine("file 5.0 6 3 4 1"), std::runtime_error);
}

TEST_F(IndexLineTests, failsForEastingValuesNotMultipleOfPixelSize)
{
	EXPECT_THROW(getParsedLine("file 3 7 4 9 5"), std::runtime_error);
}

TEST_F(IndexLineTests, failsForNorthingValuesNotMultipleOfPixelSize)
{
	EXPECT_THROW(getParsedLine("file 3 7 4 9 4"), std::runtime_error);
}

TEST_F(IndexLineTests, failsForNegativePixelSizes)
{
	EXPECT_THROW(getParsedLine("file 3 7 4 9 -1"), std::runtime_error);
}

TEST_F(IndexLineTests, failsForZeroPixelSizes)
{
	EXPECT_THROW(getParsedLine("file 3 7 4 9 0"), std::runtime_error);
}