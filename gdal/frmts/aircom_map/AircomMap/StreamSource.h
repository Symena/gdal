#pragma once

#include <iosfwd>
#include <memory>

#include <boost/filesystem/path.hpp>

#include "Warnings.h"

namespace aircom_map {

struct StreamSource
{
	virtual std::unique_ptr<std::istream> getStream(Warnings& warnings) const = 0;
	virtual std::string getStreamDescription() const = 0;
};

class FileStreamSource : public StreamSource
{
	boost::filesystem::path path;
	size_t expectedFileSize;
	
public:
	FileStreamSource(const boost::filesystem::path& path, size_t expectedFileSize)
		: path(path)
		, expectedFileSize(expectedFileSize)
	{}

	virtual std::unique_ptr<std::istream> getStream(Warnings& warnings) const override;
	virtual std::string getStreamDescription() const override;
};

}
