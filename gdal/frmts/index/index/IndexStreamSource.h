#pragma once

#include <iosfwd>
#include <memory>

#include <boost/filesystem/path.hpp>

#include "IndexWarnings.h"

struct IndexStreamSource
{
	virtual std::unique_ptr<std::istream> getStream(IndexWarnings& warnings) const = 0;
	virtual std::string getStreamDescription() const = 0;
};

class IndexFileStreamSource : public IndexStreamSource
{
	boost::filesystem::path path;
	size_t expectedFileSize;
	
public:
	IndexFileStreamSource(const boost::filesystem::path& path, size_t expectedFileSize)
		: path(path)
		, expectedFileSize(expectedFileSize)
	{}

	virtual std::unique_ptr<std::istream> getStream(IndexWarnings& warnings) const override;
	virtual std::string getStreamDescription() const override;
};