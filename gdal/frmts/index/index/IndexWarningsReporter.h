#pragma once

#include "IndexWarnings.h"

#include <cpl_error.h>

class IndexWarningsReporter
{
	IndexWarnings& warnings;

public:
	IndexWarningsReporter(IndexWarnings& warnings) : warnings(warnings) {}
	~IndexWarningsReporter()
	{
		for(const auto& warning : warnings)
			CPLError(CE_Warning, CPLE_AppDefined, warning.c_str());
	}
};