#include "Line.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem/operations.hpp>

namespace aircom_map {

Line::Line(const std::string& line, Warnings& warnings, const boost::filesystem::path& dataRoot)
{
	initializeMembers(line, dataRoot);

	checkMembers(warnings);
}

void Line::initializeMembers(const std::string& line, const boost::filesystem::path& dataRoot)
{
	boost::char_separator<char> delim(" \t\r");
	boost::tokenizer<boost::char_separator<char>> tokens(line, delim);

	const auto nrOfTokens = std::distance(tokens.begin(), tokens.end());

	if (nrOfTokens != 6)
		throw std::runtime_error("Found " + boost::lexical_cast<std::string>(nrOfTokens) + " tokens, expected 6");

	boost::filesystem::path path;

	int tokenCount = 0;
	for (const auto& token : tokens)
	{
		try
		{
			switch (tokenCount)
			{
			case 0: path = token; break;
			case 1: eastMin = boost::lexical_cast<int>(token); break;
			case 2: eastMax = boost::lexical_cast<int>(token); break;
			case 3: northMin = boost::lexical_cast<int>(token); break;
			case 4: northMax = boost::lexical_cast<int>(token); break;
			case 5: resolution = boost::lexical_cast<int>(token); break;
			default: throw std::logic_error("More tokens than should be possible");
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
			throw std::runtime_error("Token '" + token + "' could not be interpreted as integer");
		}

		++tokenCount;
	}

	if(resolution != 0)
	{
		const auto dataWidth = (eastMax - eastMin) / resolution;
		const auto dataHeight = (northMax - northMin) / resolution;
		const auto expectedFileSize = dataWidth * dataHeight * sizeof(std::int16_t);
		
		auto tilePath = path.is_absolute() ? path : (dataRoot / path);
		dataSource = std::make_shared<FileStreamSource>(tilePath, expectedFileSize);
	}
}

void Line::checkMembers(Warnings& warnings)
{
	if (resolution < 1)
	{
		warnings.add("Pixel size '%d' is not > 0", resolution);
		consistent = false;
	}
	else
	{
		if ((eastMax - eastMin) % resolution != 0)
		{
			warnings.add("Easting distance is not a multiple of the pixel size");
			consistent = false;
		}

		if ((northMax - northMin) % resolution != 0)
		{
			warnings.add("Northing distance is not a multiple of the pixel size");
			consistent = false;
		}
	}
}

}
