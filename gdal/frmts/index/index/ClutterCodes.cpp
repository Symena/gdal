#include "ClutterCodes.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

namespace aircom_map {

ClutterCodes::ClutterCodes(const boost::filesystem::path& menuFile)
	: ClutterCodes(std::ifstream(menuFile.string()))
{}

ClutterCodes::ClutterCodes(std::istream& menuFile)
{
	std::map<int, std::string> clutterCodesMap;

	readClutterMenuFile(menuFile, clutterCodesMap);

	createClutterList(clutterCodesMap);
}

void ClutterCodes::readClutterMenuFile(std::istream& menuFile, std::map<int, std::string>& clutterCodesMap)
{
	if (!menuFile.good())
		throw std::runtime_error("Clutter menu file is empty or stream is in a bad or failed state");

	maxIndex = -1;

	while (menuFile.good())
	{
		std::string line;
		std::getline(menuFile, line);

		if (line.empty())
			continue;

		clutterCodesMap.insert(readClutterLine(line));
	}
}

std::pair<int, std::string> ClutterCodes::readClutterLine(std::string line)
{
	boost::char_separator<char> delim(" \t\r");
	boost::tokenizer<boost::char_separator<char>> tokens(line, delim);

	const auto nrOfTokens = std::distance(tokens.begin(), tokens.end());

	if (nrOfTokens < 2)
		throw std::runtime_error("A clutter code must contain an index and a name");

	int tokenCount = 0;

	int index;
	std::string clutterName = "";

	for (const auto& token : tokens)
	{
		if (tokenCount == 0)
		{
			try
			{
				index = boost::lexical_cast<int>(token);

				if (index < 0 || index > 32767)
					throw std::runtime_error("Only clutter indexes in the range [0..32767] supported");

				maxIndex = index > maxIndex ? index : maxIndex;
			}
			catch (const boost::bad_lexical_cast&)
			{
				throw std::runtime_error("Infeasible index: '" + token + "' could not be interpreted as integer");
			}
		}
		else
			clutterName += token + (tokenCount < nrOfTokens - 1 ? " " : "");

		++tokenCount;
	}

	return std::make_pair(index, clutterName);
}

void ClutterCodes::createClutterList(std::map<int, std::string>& clutterCodesMap)
{
	int currentIndex = 0;

	for(const auto& clutterCode : clutterCodesMap)
	{
		while(clutterCode.first > currentIndex)
		{
			gdalClutterCodes.AddString("");
			++currentIndex;
		}

		gdalClutterCodes.AddString(clutterCode.second.c_str());
		++currentIndex;
	}
}

}
