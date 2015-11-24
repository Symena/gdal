#include "Dataset.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <cassert>

#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>

#include "RasterBand.h"
#include "Renderer.h"
#include "WarningsReporter.h"

namespace aircom_map {

namespace {

struct membuf: public std::streambuf
{
	membuf(char* begin, size_t size)
	{
		this->setg(begin, begin, begin + size);
	}
};
std::vector<Line> readLines(std::istream& indexFile, Warnings& warnings, const boost::filesystem::path& dataRoot)
{
	if (!indexFile.good())
		throw std::runtime_error("Index file is empty or stream is in a bad or failed state");

	std::vector<Line> lines;

	size_t readLines = 0;
	while (indexFile.good())
	{
		std::string line;
		std::getline(indexFile, line);
		++readLines;
		WarningsContext lineNumber(warnings, "Line %d: ", readLines);

		if (line.empty())
			continue;

		lines.emplace_back(line, warnings, dataRoot);
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

GDALDataset* Dataset::Open(GDALOpenInfo* openInfo)
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

	Warnings warnings;
	WarningsReporter warningsReporter(warnings);

	std::unique_ptr<Dataset> dataSet;

	try
	{
		WarningsContext context(warnings, boost::filesystem::absolute(indexFile).string() + ": ");
		dataSet = std::make_unique<Dataset>(indexFile, warnings);
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

bool Dataset::Identify(const boost::filesystem::path& file, std::istream& header)
{
	if (file.filename() != "index.txt")
		return false;

	try
	{
		std::string line;
		std::getline(header, line);

		Line l(line, Warnings(), file.parent_path());
	}
	catch (const std::runtime_error&)
	{
		return false;
	}

	return true;
}

Dataset::Dataset(const boost::filesystem::path& indexFile, Warnings& warnings)
	: Dataset(std::ifstream(indexFile.string()), getClutterCodeStream(indexFile), warnings, indexFile.parent_path())
{}

Dataset::Dataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, Warnings& warnings,
	const boost::filesystem::path& dataRoot)
	: Dataset(Blocks(readLines(indexFile, warnings, dataRoot)), std::move(clutterFile), dataRoot)
{}

Dataset::Dataset(Blocks blocks, std::unique_ptr<std::istream> clutterFile,
    const boost::filesystem::path& dataRoot)
	: blocks(std::move(blocks))
	, dataRoot(dataRoot)
{
	if (clutterFile)
		clutterCodes = ClutterCodes(*clutterFile);

	setBoundingBox();
	provideResolutionsAsMetadata();

	// add a raster band (required for data type, no-data value and clutter names)
	SetBand(1, new RasterBand(this, 1));
}


void Dataset::setBoundingBox()
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

void Dataset::provideResolutionsAsMetadata()
{
	for (int res : getResolutions())
	{
		auto item = std::to_string(res) + "m";
		SetMetadataItem(item.c_str(), "", "Resolutions");
	}
}

bool Dataset::render(std::int16_t* dst, DataOrientation dataOrientation, int dstWidth, int dstHeight, int dstResolution,
	MapPoint bottomLeftCornerInMeters, GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm)
{
	Warnings warnings;
	WarningsReporter warningsReporter(warnings);

	try
	{
		Renderer renderer(blocks, dst, dataOrientation, dstWidth, dstHeight, dstResolution,
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

CPLErr Dataset::IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
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
		auto message = boost::format("Index data sets only support a uniform x/y resolution. resX=%1% resY=%2% nXSize=%3% nySize=%4% nBufXSize=%5% nBufYSize=%6%")
			% resX % resY % nXSize % nYSize % nBufXSize % nBufYSize;
		CPLError(CPLErr::CE_Failure, CPLE_NotSupported, message.str().c_str());
		return CPLErr::CE_Failure;
	}

	auto algorithm = (clutterCodes
		? GDALRIOResampleAlg::GRIORA_Mode     // majority
		: GDALRIOResampleAlg::GRIORA_Bilinear);
	if (psExtraArg)
		algorithm = psExtraArg->eResampleAlg;

	bool success = render(static_cast<std::int16_t*>(pData), DataOrientation::TopDown, nBufXSize, nBufYSize, resX,
		blocks.getBoundingBox().min_corner() + MapPoint(nXOff, nYOff), algorithm, algorithm);

	return success ? CPLErr::CE_None : CPLErr::CE_Failure;
}

}
