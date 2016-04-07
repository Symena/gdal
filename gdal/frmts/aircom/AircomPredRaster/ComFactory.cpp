#include "ComFactory.h"

#include "StringUtils.h"

namespace aircom { namespace pred_raster {

namespace {

template <typename Ptr>
Ptr createComInstance(const CLSID& classID, const char* interfaceName)
{
	Ptr ptr;
	if (SUCCEEDED(ptr.CreateInstance(classID)))
		return ptr;

	LPOLESTR classIDString = L"";
	StringFromCLSID(classID, &classIDString);

	const auto msg = format("Failed to create COM interface %s using COM class ID %s",
		interfaceName, classIDString);

	throw std::runtime_error(msg);
}

}

ComFactory::ComFactory(const std::wstring& predAccessClassIDString, const std::wstring& predRasterClassIDString)
{
	if (FAILED(CLSIDFromString(predAccessClassIDString.c_str(), &predAccessClassID)))
		throw std::runtime_error(format("Invalid class ID %s for COM class PredAccess", predAccessClassIDString));
	if (FAILED(CLSIDFromString(predRasterClassIDString.c_str(), &predRasterClassID)))
		throw std::runtime_error(format("Invalid class ID %s for COM class PredRaster", predRasterClassIDString));
}

IAircomPredAccess4Ptr ComFactory::createPredAccess() const
{
	return createComInstance<IAircomPredAccess4Ptr>(predAccessClassID, "IAircomPredAccess4");
}

IPredRaster5Ptr ComFactory::createPredRaster() const
{
	return createComInstance<IPredRaster5Ptr>(predRasterClassID, "IPredRaster5");
}

}}
