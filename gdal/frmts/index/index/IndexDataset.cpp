#include "IndexDataset.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <cassert>

#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>

#include "IndexRasterBand.h"
#include "IndexRenderer.h"
#include "IndexWarningsReporter.h"

namespace {

struct membuf: public std::streambuf
{
	membuf(char* begin, size_t size)
	{
		this->setg(begin, begin, begin + size);
	}
};

std::vector<IndexLine> readLines(std::istream& indexFile, IndexWarnings& warnings)
{
	if (!indexFile.good())
		throw std::runtime_error("Index file is empty or stream is in a bad or failed state");

	std::vector<IndexLine> lines;

	size_t readLines = 0;
	while (indexFile.good())
	{
		std::string line;
		std::getline(indexFile, line);
		++readLines;
		IndexWarningsContext lineNumber(warnings, "Line %d: ", readLines);

		if (line.empty())
			continue;

		lines.emplace_back(line, warnings);
		if (!lines.back().isConsistent())
			lines.pop_back();
	}

	return lines;
}

std::unique_ptr<std::istream> getClutterCodeStream(const boost::filesystem::path& indexFile)
{
	auto menuFile = indexFile.parent_path() / "menu.txt";

	if (!boost::filesystem::exists(menuFile))
		return nullptr;

	return std::make_unique<std::ifstream>(menuFile.string());
}

}

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

IndexDataset::IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings)
	: IndexDataset(std::ifstream(indexFile.string()), getClutterCodeStream(indexFile), warnings)
{}

IndexDataset::IndexDataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, IndexWarnings& warnings)
	: IndexDataset(IndexBlocks(readLines(indexFile, warnings)), std::move(clutterFile))
{}

IndexDataset::IndexDataset(IndexBlocks blocks, std::unique_ptr<std::istream> clutterFile)
	: blocks(std::move(blocks))
{
	if (clutterFile)
		clutterCodes = IndexClutterCodes(*clutterFile);

	setBoundingBox();
	provideResolutionsAsMetadata();
	addBands();
}


void IndexDataset::setBoundingBox()
{
	const auto& bounds = getBoundingBox();
	nRasterXSize = width(bounds);
	nRasterYSize = height(bounds);

	double transformMatrix[6];
	transformMatrix[0] = bounds.min_corner().get<0>(); // minX
	transformMatrix[1] = 1; // x resolution is always 1
	transformMatrix[2] = 0;
	transformMatrix[3] = bounds.min_corner().get<1>(); // minY
	transformMatrix[4] = 0;
	transformMatrix[5] = 1; // y resolution is always 1 (rows are stored bottom-up)
	SetGeoTransform(transformMatrix);
}

void IndexDataset::provideResolutionsAsMetadata()
{
	for (int res : getResolutions())
	{
		auto item = std::to_string(res) + "m";
		SetMetadataItem(item.c_str(), "", "Resolutions");
	}
}

void IndexDataset::addBands()
{
	int i = 1;
	for (int res : getResolutions())
	{
		SetBand(i, new IndexRasterBand(this, i, res));
		++i;
	}
}

bool IndexDataset::render(std::int16_t* dst, int dstWidth, int dstHeight, int dstResolution,
	MapPoint bottomLeftCornerInMeters, GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm)
{
	IndexWarnings warnings;
	IndexWarningsReporter warningsReporter(warnings);

	try
	{
		IndexRenderer renderer(blocks, dst, dstWidth, dstHeight, dstResolution,
			bottomLeftCornerInMeters, downsamplingAlgorithm, upsamplingAlgorithm, warnings);
		renderer.render();
	}
	catch (const std::exception& e)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Rendering index data set failed: %s", e.what());
		return false;
	}

	return true;
}

CPLErr IndexDataset::IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
	void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
	int nBandCount, int *panBandMap,
	GSpacing nPixelSpace, GSpacing nLineSpace, GSpacing nBandSpace,
	GDALRasterIOExtraArg* psExtraArg)
{
	if (eRWFlag != GDALRWFlag::GF_Read)
	{
		CPLError(CPLErr::CE_Failure, CPLE_NoWriteAccess, "Index data sets can only be read from");
		return CPLErr::CE_Failure;
	}

	if (nXSize <= 0 || nYSize <= 0 || nBufXSize <= 0 || nBufYSize <= 0 || pData == nullptr)
	{
		CPLError(CPLErr::CE_Failure, CPLE_IllegalArg, "Invalid arguments");
		return CPLErr::CE_Failure;
	}

	if (eBufType != GDALDataType::GDT_Int16 ||
		(nPixelSpace != 0 && nPixelSpace != sizeof(std::int16_t)) ||
		(nLineSpace != 0 && nLineSpace != nBufXSize * nPixelSpace))
	{
		CPLError(CPLErr::CE_Failure, CPLE_NotSupported, "Index data sets only support reading into contiguous 16-bit buffers");
		return CPLErr::CE_Failure;
	}

	// bands are completely ignored (nBandCount, panBandMap, nBandSpace)

	const int resX = std::lround(static_cast<double>(nXSize /* in meters */) / nBufXSize);
	const int resY = std::lround(static_cast<double>(nYSize /* in meters */) / nBufYSize);
	if (resX != resY)
	{
		CPLError(CPLErr::CE_Failure, CPLE_NotSupported, "Index data sets only support a uniform x/y resolution");
		return CPLErr::CE_Failure;
	}

	auto algorithm = (psExtraArg ? psExtraArg->eResampleAlg : GDALRIOResampleAlg::GRIORA_NearestNeighbour);
	bool success = render(static_cast<std::int16_t*>(pData), nBufXSize, nBufYSize, resX,
		blocks.getBoundingBox().min_corner() + MapPoint(nXOff, nYOff), algorithm, algorithm);

	return success ? CPLErr::CE_None : CPLErr::CE_Failure;
}
