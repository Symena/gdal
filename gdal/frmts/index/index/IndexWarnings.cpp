#include "IndexWarnings.h"


void IndexWarnings::add(const std::string& warning)
{
	warnings.push_back(currentContext + warning);
}

void IndexWarnings::pushContext(const std::string& context)
{
	this->currentContext += context;
	activeContexts.push_back(context);
}

void IndexWarnings::popContext()
{
	activeContexts.pop_back();

	currentContext = "";

	for(const auto& context : activeContexts)
		currentContext += context;
}
