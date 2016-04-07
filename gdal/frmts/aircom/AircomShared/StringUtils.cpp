#include "StringUtils.h"

#include <Windows.h>
#include <vector>

namespace aircom {

std::string ws2ns(const std::wstring &ws)
{
	if(ws.empty())
		return "";

	BOOL dummy;
	int needed = WideCharToMultiByte(CP_ACP, 0,
		ws.data(), static_cast<int>(ws.size()), nullptr, 0, nullptr, &dummy);
	if(needed == 0)
		throw std::runtime_error("conversion error");

	std::vector<char> out(needed);
	WideCharToMultiByte(CP_ACP, 0, ws.data(), static_cast<int>(ws.size()),
		&out[0], static_cast<int>(out.size()), 0, &dummy);

	return std::string(out.begin(), out.end());
}

std::wstring ns2ws(const std::string &ns)
{
	if(ns.empty())
		return L"";

	int needed = MultiByteToWideChar(CP_ACP, 0,
		ns.data(), static_cast<int>(ns.size()), nullptr, 0);
	if(needed == 0)
		throw std::runtime_error("conversion error");

	std::vector<WCHAR> out(needed);
	MultiByteToWideChar(CP_ACP, 0, ns.data(), static_cast<int>(ns.size()),
		&out[0], static_cast<int>(out.size()));

	return std::wstring(out.begin(), out.end());
}

}
