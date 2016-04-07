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

ComFactory::ComFactory(const std::wstring& predRasterClassIDString)
{
	if (predRasterClassIDString == L"{98A67A67-9DAB-407A-AA74-AEF504C165EE}")
	{
		CLSIDFromString(L"{A6EC9A6C-3262-4DEE-AA29-2DEC97FBED2D}", &predAccessClassID);
		CLSIDFromString(predRasterClassIDString.c_str(), &predRasterClassID);
	}
	else
		throw std::runtime_error(format("Unsupported PredRaster COM class ID %s", predRasterClassIDString));
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
