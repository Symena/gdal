#pragma once

#include <string>
#include <deque>
#include <boost/format.hpp>

namespace aircom_map {

class Warnings
{
	std::deque<std::string> warnings;
	std::deque<std::string> activeContexts;
	std::string currentContext;

public:
	void add(const std::string& warning);

	template <class... ArgumentTypes>
	void add(const std::string& formatString, const ArgumentTypes& ... args)
	{
		boost::format format(formatString);
		internalFormat(format, args...);
		add(boost::str(format));
	}

	size_t size() const { return warnings.size(); }
	std::deque<std::string>::const_iterator begin() const { return warnings.cbegin(); }
	std::deque<std::string>::const_iterator end() const { return warnings.cend(); }

private:
	void pushContext(const std::string& context);
	
	template <class... ArgumentTypes>
	void pushContext(const std::string& contextFormatString, const ArgumentTypes& ... args)
	{
		boost::format format(contextFormatString);
		internalFormat(format, args...);
		pushContext(boost::str(format));
	}
	
	void popContext();

	void internalFormat(boost::format& /*format*/) {}

	template <class T, class... OtherTs>
	void internalFormat(boost::format& format, const T& arg, const OtherTs& ... otherArgs)
	{
		format % arg;
		internalFormat(format, otherArgs...);
	}

	friend class WarningsContext;
};

class WarningsContext
{
	Warnings& warnings;

public:
	WarningsContext(Warnings& warnings, const std::string& context) : warnings(warnings)
	{
		this->warnings.pushContext(context);
	}

	template <class... ArgumentTypes>
	WarningsContext(Warnings& warnings, const std::string& contextFormatString, const ArgumentTypes& ... args) : warnings(warnings)
	{
		this->warnings.pushContext(contextFormatString, args...);
	}

	~WarningsContext()
	{
		warnings.popContext();
	}
};

}
