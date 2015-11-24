#include "Warnings.h"

namespace aircom_map {

void Warnings::add(const std::string& warning)
{
	warnings.push_back(currentContext + warning);
}

void Warnings::pushContext(const std::string& context)
{
	this->currentContext += context;
	activeContexts.push_back(context);
}

void Warnings::popContext()
{
	activeContexts.pop_back();

	currentContext = "";

	for(const auto& context : activeContexts)
		currentContext += context;
}

}
