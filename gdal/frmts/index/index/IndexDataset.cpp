#include "IndexDataset.h"

#include <iostream>
#include <sstream>
#include <memory>

#include <boost/filesystem.hpp>

#include "IndexLine.h"
#include "IndexWarnings.h"
#include "IndexWarningsReporter.h"

IndexDataset::IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings) 
	: IndexDataset(std::ifstream(indexFile.string()), warnings)
{}


IndexDataset::IndexDataset(std::istream& indexFile, IndexWarnings& warnings)
{
	if(!indexFile.good())
		throw std::runtime_error("Index file is empty or stream is in a bad or failed state");

	std::vector<IndexLine> lines;

	size_t readLines = 0;
	while(indexFile.good())
	{
		std::string line;
		std::getline(indexFile, line);
		++readLines;
		IndexWarningsContext lineNumber(warnings, "Line %d: ", readLines);

		if(line.empty())
			continue;

		lines.emplace_back(line, warnings);
	}

	std::cout << "Read "<<lines.size()<<" lines from index file\n";
}

struct membuf : public std::streambuf
{
	membuf(char* begin, size_t size)
	{
		this->setg(begin, begin, begin + size);
	}
};

GDALDataset* IndexDataset::Open(GDALOpenInfo* openInfo)
{
	if(openInfo->pszFilename == nullptr || openInfo->pabyHeader == nullptr || openInfo->fpL == nullptr)
		return nullptr;

	if(!openInfo->bStatOK)
		return nullptr;

	if (openInfo->eAccess != GA_ReadOnly)
	{
		CPLError(CE_Failure, CPLE_NotSupported, "The Index driver only supports readonly access to existing datasets.\n");
		return nullptr;
	}

	const boost::filesystem::path indexFile = openInfo->pszFilename;
	
	membuf sbuf(reinterpret_cast<char*>(openInfo->pabyHeader), openInfo->nHeaderBytes);
	std::istream header(&sbuf);

	if (!Identify(indexFile, header))
		return nullptr;

	IndexWarnings warnings;
	IndexWarningsReporter warningsReporter(warnings);

	std::unique_ptr<IndexDataset> dataSet; 
	
	try
	{
		IndexWarningsContext context(warnings, boost::filesystem::absolute(indexFile).string()+": ");
		dataSet = std::make_unique<IndexDataset>(indexFile, warnings);
	}
	catch (const std::runtime_error& e)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Reading index file %s failed: %s", openInfo->pszFilename, e.what());
		return nullptr;
	}

	//TODO: create band information
		

	dataSet->SetDescription(openInfo->pszFilename);
	dataSet->TryLoadXML();

	return dataSet.release();
}

bool IndexDataset::Identify(const boost::filesystem::path& file, std::istream& header)
{
	if(file.filename() != "index.txt")
		return false;

	try
	{
		std::string line;
		std::getline(header, line);

		IndexLine l(line, IndexWarnings());
	}
	catch(const std::runtime_error&)
	{
		return false;
	}

	return true;
}
