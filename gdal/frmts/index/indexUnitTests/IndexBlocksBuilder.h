#pragma once

#include <string>
#include <sstream>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include "IndexLine.h"
#include "IndexBlocks.h"

#include "IndexStreamSource.h"

class VectorBackedStreamSource: public IndexStreamSource
{
	std::vector<std::int16_t> entries;

public:
	VectorBackedStreamSource(std::vector<std::int16_t> entries)
		: entries(std::move(entries))
	{}

	virtual std::unique_ptr<std::istream> getStream() const override
	{
		typedef boost::iostreams::basic_array<char> Device;
		auto inputStream = std::make_unique<boost::iostreams::stream<Device>>(const_cast<char*>(reinterpret_cast<const char*>(entries.data())), entries.size()*sizeof(std::int16_t));

		inputStream->exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

		return inputStream;
	}

	virtual std::string getStreamDescription() const override
	{
		return "VectorBackedStream";
	}
};

class IndexBlocksBuilder
{
	int pixelSize = 1;

	struct ProtoLine
	{
		int eastMin = 0;
		int eastMax = 0;
		int northMin = 0;
		int northMax = 0;
		std::shared_ptr<IndexStreamSource> data;
	};

	std::vector<ProtoLine> protoLines;

public:

	IndexBlocksBuilder& addTile() { protoLines.emplace_back(); return *this;}
	
	IndexBlocksBuilder& from(int eastMin, int northMin)
	{
		protoLines.back().eastMin = eastMin;
		protoLines.back().northMin = northMin;

		return *this;
	}

	IndexBlocksBuilder& to(int eastMax, int northMax)
	{
		protoLines.back().eastMax = eastMax;
		protoLines.back().northMax = northMax;

		return *this;
	}

	IndexBlocksBuilder& withData(std::vector<std::int16_t> data)
	{
		protoLines.back().data = std::make_shared<VectorBackedStreamSource>(std::move(data));

		return *this;
	}

	IndexBlocksBuilder& setPixelSize(int pixelSize)
	{
		this->pixelSize = pixelSize;

		return *this;
	}

	IndexBlocks create()
	{
		std::vector<IndexLine> lines;
		lines.reserve(protoLines.size());

		for(const auto& protoLine : protoLines)
			lines.emplace_back(protoLine.eastMin, protoLine.eastMax, protoLine.northMin, protoLine.northMax, pixelSize, protoLine.data);

		return IndexBlocks(lines);
	}
};