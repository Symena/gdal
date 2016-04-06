#pragma once

#include <string>
#include <sstream>

#include <boost/endian/conversion.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include "Blocks.h"

namespace aircom_map {

class VectorBackedStreamSource: public StreamSource
{
	std::vector<std::int16_t> entries;

public:
	explicit VectorBackedStreamSource(std::vector<std::int16_t> entries)
		: entries(std::move(entries))
	{}

	virtual std::unique_ptr<std::istream> getStream(Warnings& /*warnings*/) const override
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

class BlocksBuilder
{
	struct ProtoLine
	{
		int eastMin = 0;
		int eastMax = 0;
		int northMin = 0;
		int northMax = 0;
		int resolution = 1;
		std::shared_ptr<StreamSource> data;
	};

	std::vector<ProtoLine> protoLines;

public:
	BlocksBuilder& addBlock()
	{
		protoLines.emplace_back();
		return *this;
	}

	Block getLastBlock() const
	{
		auto& l = protoLines.back();
		return Block(Line(l.eastMin, l.eastMax, l.northMin, l.northMax, l.resolution, l.data), 0);
	}
	
	BlocksBuilder& from(int eastMin, int northMin)
	{
		protoLines.back().eastMin = eastMin;
		protoLines.back().northMin = northMin;

		return *this;
	}

	BlocksBuilder& to(int eastMax, int northMax)
	{
		protoLines.back().eastMax = eastMax;
		protoLines.back().northMax = northMax;

		return *this;
	}

	BlocksBuilder& resolution(int resolution)
	{
		protoLines.back().resolution = resolution;
		return *this;
	}

	BlocksBuilder& withData(std::vector<std::int16_t> topDownData)
	{
		for (auto& entry : topDownData)
			boost::endian::native_to_big_inplace(entry);

		protoLines.back().data = std::make_shared<VectorBackedStreamSource>(std::move(topDownData));

		return *this;
	}

	Blocks create() const
	{
		std::vector<Line> lines;
		lines.reserve(protoLines.size());

		for (auto& protoLine : protoLines)
			lines.emplace_back(protoLine.eastMin, protoLine.eastMax, protoLine.northMin, protoLine.northMax, protoLine.resolution, protoLine.data);

		return Blocks(lines);
	}
};

}
