#include <gmock/gmock.h>

#include <boost/filesystem/path.hpp>

#include "IndexDataset.h"

class IndexDatasetTests : public ::testing::Test
{
	boost::filesystem::path fileName = "index.txt";
	std::stringstream header;

public:
	void setFileName(const std::string& fileName) { this->fileName = fileName; }
	void setHeader(const std::string& header) { this->header << header; }

	bool identify()
	{
		return IndexDataset::Identify(fileName, header);
	}
};


TEST_F(IndexDatasetTests, IdentificationFailsOnWrongFileName)
{
	setFileName("afile.txt");

	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetTests, IdentificationFailsOnEmptyHeader)
{
	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetTests, IdentificationFailsOnMalformedHeader)
{
	setHeader("asdklöfjöasldkfj");

	EXPECT_FALSE(identify());
}

TEST_F(IndexDatasetTests, IdentificationWorksIfFirstLineCanBeParsed)
{
	setHeader("file 0 1 0 1 1 \n garbage");

	EXPECT_TRUE(identify());
}