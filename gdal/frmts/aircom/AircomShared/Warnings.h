#pragma once

#include <deque>

#include "StringUtils.h"

namespace aircom {

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
		add(format(formatString, args...));
	}

	size_t size() const { return warnings.size(); }
	auto begin() const { return warnings.cbegin(); }
	auto end() const { return warnings.cend(); }

private:
	void pushContext(const std::string& context);
	
	template <class... ArgumentTypes>
	void pushContext(const std::string& contextFormatString, const ArgumentTypes& ... args)
	{
		pushContext(format(contextFormatString, args...));
	}
	
	void popContext();

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
		this->warnings.pushContext(format(contextFormatString, args...));
	}

	~WarningsContext()
	{
		warnings.popContext();
	}
};

}
