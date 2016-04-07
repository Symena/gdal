#include "AircomPredRaster/ComFactory.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

TEST(ComFactory, ctor_throwsOnInvalidClassID)
{
	EXPECT_THROW(ComFactory(L"123", L"456"), std::runtime_error);
}

}}
