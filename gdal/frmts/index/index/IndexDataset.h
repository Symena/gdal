#pragma once

#include "gdal_pam.h"

#include <iosfwd>
#include <vector>
#include <memory>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include "IndexBlocks.h"
#include "IndexClutterCodes.h"
#include "IndexDataOrientation.h"
#include "IndexWarnings.h"

class IndexLine;

class IndexDataset: public GDALPamDataset
{
public:
	IndexDataset(const boost::filesystem::path& indexFile, IndexWarnings& warnings);
	IndexDataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, IndexWarnings& warnings,
		const boost::filesystem::path& dataRoot);
	IndexDataset(IndexBlocks blocks, std::unique_ptr<std::istream> clutterFile,
	    const boost::filesystem::path& dataRoot);

	static GDALDataset* Open(GDALOpenInfo* openInfo);
	static bool Identify(const boost::filesystem::path& file, std::istream& header);

	const auto& getBoundingBox() const { return blocks.getBoundingBox(); }
	const auto& getResolutions() const { return blocks.getResolutions(); }

	boost::optional<IndexClutterCodes>& getClutterCodes() { return clutterCodes; }
	const boost::filesystem::path dataRoot;

	bool render(std::int16_t* dst, IndexDataOrientation dataOrientation, int dstWidth, int dstHeight, int dstResolution,
		MapPoint bottomLeftCornerInMeters,
		GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm);

protected:
	virtual CPLErr IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
		void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
		int nBandCount, int *panBandMap,
		GSpacing nPixelSpace, GSpacing nLineSpace, GSpacing nBandSpace,
		GDALRasterIOExtraArg* psExtraArg) override;

private:
	IndexBlocks blocks;
	boost::optional<IndexClutterCodes> clutterCodes;

	void setBoundingBox();
	void provideResolutionsAsMetadata();
};
