#include "IndexDataset.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <cassert>

#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>

#include "IndexLine.h"
#include "IndexWarnings.h"
#include "IndexWarningsReporter.h"
#include "IndexRasterBand.h"
#include "IndexBlocks.h"

struct membuf: public std::streambuf
{
	membuf(char* begin, size_t size)
	{
		this->setg(begin, begin, begin + size);
	}
};

GDALDataset* IndexDataset::Open(GDALOpenInfo* openInfo)
{
	if (openInfo->pszFilename == nullptr || openInfo->pabyHeader == nullptr || openInfo->fpL == nullptr)
		return nullptr;

	if (!openInfo->bStatOK)
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
		IndexWarningsContext context(warnings, boost::filesystem::absolute(indexFile).string() + ": ");
		dataSet = std::make_unique<IndexDataset>(indexFile, warnings);
	}
	catch (const std::runtime_error& e)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Reading index file %s failed: %s", openInfo->pszFilename, e.what());
		return nullptr;
	}

	dataSet->SetDescription(openInfo->pszFilename);
	dataSet->TryLoadXML();

	dataSet->oOvManager.Initialize(dataSet.get(), openInfo->pszFilename);

	return dataSet.release();
}

bool IndexDataset::Identify(const boost::filesystem::path& file, std::istream& header)
{
	if (file.filename() != "index.txt")
		return false;

	try
	{
		std::string line;
		std::getline(header, line);

		IndexLine l(line, IndexWarnings());
	}
	catch (const std::runtime_error&)
	{
		return false;
	}

	return true;
}

std::unique_ptr<std::istream> getClutterCodeStream(const boost::filesystem::path& indexFile)
{
	auto menuFile = indexFile.parent_path() / "menu.txt";

	if(!boost::filesystem::exists(menuFile))
		return {};

	return std::make_unique<std::ifstream>(menuFile.string());
}

IndexDataset::IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings) 
	: IndexDataset(std::ifstream(indexFile.string()), getClutterCodeStream(indexFile), warnings)
{}

IndexDataset::IndexDataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, IndexWarnings& warnings)
{
	if(!indexFile.good())
		throw std::runtime_error("Index file is empty or stream is in a bad or failed state");

	std::vector<IndexLine> lines;
	int bestPixelSquareSize = -1;

	readLines(indexFile,lines, bestPixelSquareSize, warnings);
	filterUnusableLines(lines, bestPixelSquareSize);

	provideResolutionsAsMetadata(lines);

	auto blocks = IndexBlocks(lines);

 	setRasterSizes(blocks);

	SetBand(1, new IndexRasterBand(this, std::move(blocks), readClutterCodes(std::move(clutterFile))));
}

void IndexDataset::setRasterSizes(const IndexBlocks& blocks)
{
	nRasterXSize = static_cast<int>(blocks.getNrBlocksX()*blocks.getBlockXSize());
	nRasterYSize = static_cast<int>(blocks.getNrBlocksY()*blocks.getBlockYSize());
}

void IndexDataset::filterUnusableLines(std::vector<IndexLine>& lines, int targetPixelSquareSize)
{
	std::vector<IndexLine> usableLines;
	usableLines.reserve(lines.size());

	for(const auto& line : lines)
		if(line.isConsistent() && line.getPixelSquareSize() == targetPixelSquareSize)
			usableLines.push_back(line);

	std::swap(lines, usableLines);
}

void IndexDataset::readLines(std::istream& indexFile, std::vector<IndexLine>& lines, int& bestPixelSquareSize, IndexWarnings& warnings)
{
	size_t readLines = 0;
	bestPixelSquareSize = std::numeric_limits<int>::max();
	while (indexFile.good())
	{
		std::string line;
		std::getline(indexFile, line);
		++readLines;
		IndexWarningsContext lineNumber(warnings, "Line %d: ", readLines);

		if (line.empty())
			continue;

		lines.emplace_back(line, warnings);

		const auto& readLine = lines.back();
		if (readLine.isConsistent())
			bestPixelSquareSize = std::min(bestPixelSquareSize, readLine.getPixelSquareSize());
	}
}

boost::optional<IndexClutterCodes> IndexDataset::readClutterCodes(std::unique_ptr<std::istream> clutterFile)
{
	if(!clutterFile)
		return {};

	return IndexClutterCodes(*clutterFile);
}

void IndexDataset::provideResolutionsAsMetadata(const std::vector<IndexLine>& lines)
{
	// resolution to number of tiles
	std::map<int, int> resolutions;

	for (const auto& line : lines)
		++resolutions[line.getPixelSquareSize()];

	for (const auto& res : resolutions)
	{
		auto sRes = std::to_string(res.first) + "m";
		auto sSize = std::to_string(res.second) + " tiles";

		SetMetadataItem(sRes.c_str(), sSize.c_str(), "Resolutions");
	}
}
