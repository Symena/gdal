#include <gmock/gmock.h>

#include "IndexLine.h"

TEST(IndexLine, parsesSuccessfully)
{
	IndexLine("file 0 0 0 0 0");
}