#pragma once

#include <streambuf>

namespace aircom_map {

struct membuf : public std::streambuf
{
	membuf(char* begin, size_t size)
	{
		this->setg(begin, begin, begin + size);
	}
};

}
