#include "IndexLine.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

IndexLine::IndexLine(const std::string& line, IndexWarnings& warnings)
{
	initializeMembers(line);

	checkMembers(warnings);
}

void IndexLine::initializeMembers(const std::string& line)
{
	boost::char_separator<char> delim(" \t\r");
	boost::tokenizer<boost::char_separator<char>> tokens(line, delim);

	const auto nrOfTokens = std::distance(tokens.begin(), tokens.end());

	if (nrOfTokens != 6)
		throw std::runtime_error("Found " + boost::lexical_cast<std::string>(nrOfTokens) + " tokens, expected 6");

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
			case 5: pixelSquareSize = boost::lexical_cast<int>(token); break;
			default: throw std::logic_error("More tokens than should be possible");
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
			throw std::runtime_error("Token '" + token + "' could not be interpreted as integer");
		}

		++tokenCount;
	}
}

void IndexLine::checkMembers(IndexWarnings& warnings)
{
	if (pixelSquareSize < 1)
	{
		warnings.add("Pixel size '%d' is not > 0", pixelSquareSize);
		consistent = false;
	}
	else
	{
		if ((eastMax - eastMin) % pixelSquareSize != 0)
		{
			warnings.add("Easting distance is not a multiple of the pixel size");
			consistent = false;
		}

		if ((northMax - northMin) % pixelSquareSize != 0)
		{
			warnings.add("Northing distance is not a multiple of the pixel size");
			consistent = false;
		}
	}
}
