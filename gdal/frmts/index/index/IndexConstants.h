#pragma once

#include <cstdint>

#include <boost/endian/conversion.hpp>

static const std::int16_t ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES_LITTLE_ENDIAN = -9999;
static const std::int16_t ASSET_MAGIC_CONSTANT_FOR_UNDEFINED_VALUES_BIG_ENDIAN = boost::endian::native_to_big(static_cast<std::int16_t>(-9999));
