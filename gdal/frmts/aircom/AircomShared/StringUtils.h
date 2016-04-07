#pragma once

#include <string>
#include <boost/format.hpp>

namespace aircom {

std::string ws2ns(const std::wstring &ws);
std::wstring ns2ws(const std::string &ns);

namespace internals
{
	template <class TChar>
	void formatInternal(boost::basic_format<TChar>& /*format*/) {}

	template <class TChar, class T, class... OtherTs>
	void formatInternal(boost::basic_format<TChar>& format, const T& arg, const OtherTs& ... otherArgs)
	{
		format % arg;
		formatInternal(format, otherArgs...);
	}

	#pragma region Specialization for narrow format string and wide arguments
	template <class... OtherTs>
	void formatInternal(boost::format& format, const std::wstring& arg, const OtherTs& ... otherArgs)
	{
		format % ws2ns(arg);
		formatInternal(format, otherArgs...);
	}

	template <class... OtherTs>
	void formatInternal(boost::format& format, const wchar_t* arg, const OtherTs& ... otherArgs)
	{
		formatInternal(format, std::wstring(arg), otherArgs...);
	}

	template <class... OtherTs>
	void formatInternal(boost::format& format, wchar_t* arg, const OtherTs& ... otherArgs)
	{
		formatInternal(format, std::wstring(arg), otherArgs...);
	}
	#pragma endregion

	#pragma region Specialization for wide format string and narrow arguments
	template <class... OtherTs>
	void formatInternal(boost::wformat& format, const std::string& arg, const OtherTs& ... otherArgs)
	{
		format % ns2ws(arg);
		formatInternal(format, otherArgs...);
	}

	template <class... OtherTs>
	void formatInternal(boost::wformat& format, const char* arg, const OtherTs& ... otherArgs)
	{
		formatInternal(format, std::string(arg), otherArgs...);
	}

	template <class... OtherTs>
	void formatInternal(boost::wformat& format, char* arg, const OtherTs& ... otherArgs)
	{
		formatInternal(format, std::string(arg), otherArgs...);
	}
	#pragma endregion
}


// Returns a formatted string (wide or narrow), based on boost::format.
template <class TChar, class... ArgumentTypes>
std::basic_string<TChar> format(const TChar* formatString, const ArgumentTypes& ... args)
{
	boost::basic_format<TChar> format(formatString);
	internals::formatInternal(format, args...);
	return format.str();
}

// Returns a formatted string (wide or narrow), based on boost::format.
template <class TChar, class... ArgumentTypes>
std::basic_string<TChar> format(const std::basic_string<TChar>& formatString, const ArgumentTypes& ... args)
{
	return format(formatString.c_str(), args...);
}

}
