#include "IndexStreamSource.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include <cpl_error.h>

std::unique_ptr<std::istream> IndexFileStreamSource::getStream(IndexWarnings& warnings) const 
{
	if(!boost::filesystem::exists(path))
	{
		warnings.add("File %1% could not be found", path);

		return {};
	}

	auto actualFileSize = boost::filesystem::file_size(path);

	if (actualFileSize != expectedFileSize)
	{
		warnings.add("File %1% has a size of %2% bytes, but should have %3% bytes, it will not be used", path, actualFileSize, expectedFileSize);

		return {};
	}

	auto dataFile = std::make_unique<std::ifstream>(path.string(), std::ios::binary);

	if (!(*dataFile))
	{
		warnings.add("Could not open file %1%", path);

		return {};
	}

	dataFile->exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

	return dataFile;
}

std::string IndexFileStreamSource::getStreamDescription() const 
{
	return path.string();
}
