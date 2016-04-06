#pragma once

#include "Warnings.h"

#include <cpl_error.h>

namespace aircom {

class WarningsReporter
{
	Warnings& warnings;

public:
	WarningsReporter(Warnings& warnings) : warnings(warnings) {}
	~WarningsReporter()
	{
		for(const auto& warning : warnings)
			CPLError(CE_Warning, CPLE_AppDefined, warning.c_str());
	}
};

}
