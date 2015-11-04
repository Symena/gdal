#include "IndexStreamSource.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>

#include <cpl_error.h>

std::unique_ptr<std::istream> IndexFileStreamSource::getStream() const
{
	auto actualFileSize = boost::filesystem::file_size(path);

	if (actualFileSize != expectedFileSize)
	{
		CPLError(CE_Failure, CPLE_AppDefined, (boost::filesystem::absolute(path).string() + " has unexpected size").c_str());
		return{};
	}

	auto dataFile = std::make_unique<std::ifstream>(path.string(), std::ios::binary);

	if (!(*dataFile))
	{
		CPLError(CE_Warning, CPLE_AppDefined, ("Could not open tile file " + boost::filesystem::absolute(path).string()).c_str());
		return {};
	}

	dataFile->exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);

	return dataFile;
}

std::string IndexFileStreamSource::getStreamDescription() const 
{
	return path.string();
}
