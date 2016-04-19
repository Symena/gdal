#pragma once

#include "Dataset.h"

namespace aircom { namespace pred_raster {

class RasterBand : public GDALRasterBand
{
public:
	struct RowSegment { int start, end; };

	RasterBand(Dataset* owningDataSet, Point sizeInPixels,
		int bandIndex, std::shared_ptr<ApiWrapper> apiWrapper,
		unsigned long sectionNum, const SectionInfo& sectionInfo);

	virtual GDALColorInterp GetColorInterpretation() override;

	virtual CPLErr SetNoDataValue(double value) override;
	virtual double GetNoDataValue(int* pbSuccess = nullptr) override;

	virtual double GetScale(int* pbSuccess = nullptr) override;

	bool readBlock(IPredRasterTileIteratorPtr tileIterator, Point blockIndex, void* data) const;
	void postProcessBlock(Point blockIndex, void* data);

protected:
	virtual CPLErr IReadBlock(int nXBlockOff, int nYBlockOff, void* pImage) override;

private:
	std::shared_ptr<ApiWrapper> apiWrapper;
	const unsigned long sectionNum;
	boost::optional<double> noDataValue;
	std::vector<RowSegment> rowSegmentsInsidePredictionRadius;

	IPredRaster5Ptr getPredRaster() { return apiWrapper->getPredRaster(); }

	size_t getNumPixelsPerBlock() const { return size_t(nBlockXSize) * nBlockYSize; }

	void readTile(IRasterTilePtr tile, size_t numPixels, void* data) const;

	void fillNoDataBlock(void* blockData) const;
	void fillPartialBlock(IRasterTilePtr tile, void* blockData) const;

	void computeRowSegmentsInsidePredictionRadius();
};

}}
