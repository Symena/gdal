#include "IndexLine.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

IndexLine::IndexLine(std::istream& indexFile)
{
	checkStreamValidity(indexFile);

	initializeMembers(indexFile);

	checkMembers();
}

void IndexLine::checkStreamValidity(const std::istream& indexFile)
{
	if (!indexFile)
		throw std::runtime_error("Stream to index file is not valid");
}

void IndexLine::initializeMembers(std::istream& indexFile)
{
	std::string line;
	std::getline(indexFile, line);

	boost::char_separator<char> delim(" \t");
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
			default:
				throw std::logic_error("More tokens than should be possible");
			}

		}
		catch (const boost::bad_lexical_cast&)
		{
			throw std::runtime_error("Token '" + token + "' could not be interpreted as integer");
		}

		++tokenCount;
	}
}

void IndexLine::checkMembers()
{
	if (pixelSquareSize < 1)
		throw std::runtime_error("Only non-negative pixel sizes are supported");

	if ((eastMax - eastMin) % pixelSquareSize != 0)
		throw std::runtime_error("Easting values are not a multiple of the pixel size");

	if ((northMax - northMin) % pixelSquareSize != 0)
		throw std::runtime_error("Northing values are not a multiple of the pixel size");
}
