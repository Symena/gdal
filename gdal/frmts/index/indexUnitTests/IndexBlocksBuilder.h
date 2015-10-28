#pragma once

#include <string>
#include <sstream>

#include "IndexWarnings.h"
#include "IndexLine.h"
#include "IndexBlocks.h"

class IndexBlocksBuilder
{
	int pixelSize = 1;
	IndexWarnings w;
	std::vector<IndexLine> lines;

public:
	IndexBlocksBuilder& addBlock(int eastMin, int eastMax, int northMin, int northMax, const std::string& filename = "file")
	{
		std::stringstream lineStream;
		lineStream << filename << " " << eastMin << " " << eastMax << " " << northMin << " " << northMax << " " << pixelSize << "\n";

		std::string s;
		std::getline(lineStream, s);

		lines.emplace_back(s, w);

		return *this;
	}

	IndexBlocksBuilder& setPixelSize(int pixelSize)
	{
		this->pixelSize = pixelSize;

		return *this;
	}

	IndexBlocks create()
	{
		return IndexBlocks(lines);
	}
};