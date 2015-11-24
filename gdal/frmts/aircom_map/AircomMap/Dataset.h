#pragma once

#include "gdal_pam.h"

#include <iosfwd>
#include <vector>
#include <memory>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include "Blocks.h"
#include "ClutterCodes.h"
#include "DataOrientation.h"
#include "Warnings.h"

namespace aircom_map {

class Line;

class Dataset: public GDALPamDataset
{
public:
	Dataset(const boost::filesystem::path& indexFile, Warnings& warnings);
	Dataset(std::istream& indexFile, std::unique_ptr<std::istream> clutterFile, Warnings& warnings,
		const boost::filesystem::path& dataRoot);
	Dataset(Blocks blocks, std::unique_ptr<std::istream> clutterFile,
	    const boost::filesystem::path& dataRoot);

	static GDALDataset* Open(GDALOpenInfo* openInfo);
	static bool Identify(const boost::filesystem::path& file, std::istream& header);

	const auto& getBoundingBox() const { return blocks.getBoundingBox(); }
	const auto& getResolutions() const { return blocks.getResolutions(); }

	boost::optional<ClutterCodes>& getClutterCodes() { return clutterCodes; }
	const boost::filesystem::path dataRoot;

	bool render(std::int16_t* dst, DataOrientation dataOrientation, int dstWidth, int dstHeight, int dstResolution,
		MapPoint bottomLeftCornerInMeters,
		GDALRIOResampleAlg downsamplingAlgorithm, GDALRIOResampleAlg upsamplingAlgorithm);

protected:
	virtual CPLErr IRasterIO(GDALRWFlag eRWFlag, int nXOff, int nYOff, int nXSize, int nYSize,
		void* pData, int nBufXSize, int nBufYSize, GDALDataType eBufType,
		int nBandCount, int *panBandMap,
		GSpacing nPixelSpace, GSpacing nLineSpace, GSpacing nBandSpace,
		GDALRasterIOExtraArg* psExtraArg) override;

private:
	Blocks blocks;
	boost::optional<ClutterCodes> clutterCodes;

	void setBoundingBox();
	void provideResolutionsAsMetadata();
};

}
