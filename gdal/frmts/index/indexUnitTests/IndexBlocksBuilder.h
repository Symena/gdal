#pragma once

#include <string>
#include <sstream>

#include <boost/endian/conversion.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include "IndexBlocks.h"

class VectorBackedStreamSource: public IndexStreamSource
{
	std::vector<std::int16_t> entries;

public:
	explicit VectorBackedStreamSource(std::vector<std::int16_t> entries)
		: entries(std::move(entries))
	{}

	virtual std::unique_ptr<std::istream> getStream(IndexWarnings& /*warnings*/) const override
	{
		using Device = boost::iostreams::basic_array<char>;
		auto inputStream = std::make_unique<boost::iostreams::stream<Device>>(const_cast<char*>(reinterpret_cast<const char*>(entries.data())),
			entries.size() * sizeof(std::int16_t));

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
	struct ProtoLine
	{
		int eastMin = 0;
		int eastMax = 0;
		int northMin = 0;
		int northMax = 0;
		int resolution = 1;
		std::shared_ptr<IndexStreamSource> data;
	};

	std::vector<ProtoLine> protoLines;

public:
	IndexBlocksBuilder& addBlock()
	{
		protoLines.emplace_back();
		return *this;
	}

	IndexBlock getLastBlock() const
	{
		auto& l = protoLines.back();
		return IndexBlock(IndexLine(l.eastMin, l.eastMax, l.northMin, l.northMax, l.resolution, l.data), 0);
	}
	
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

	IndexBlocksBuilder& resolution(int resolution)
	{
		protoLines.back().resolution = resolution;
		return *this;
	}

	IndexBlocksBuilder& withData(std::vector<std::int16_t> topDownData)
	{
		for (auto& entry : topDownData)
			boost::endian::native_to_big_inplace(entry);

		protoLines.back().data = std::make_shared<VectorBackedStreamSource>(std::move(topDownData));

		return *this;
	}

	IndexBlocks create() const
	{
		std::vector<IndexLine> lines;
		lines.reserve(protoLines.size());

		for (auto& protoLine : protoLines)
			lines.emplace_back(protoLine.eastMin, protoLine.eastMax, protoLine.northMin, protoLine.northMax, protoLine.resolution, protoLine.data);

		return IndexBlocks(lines);
	}
};
