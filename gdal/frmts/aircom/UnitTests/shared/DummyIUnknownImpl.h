#pragma once

#include <unknwnbase.h>

namespace aircom {

template<class IUNKNOWN>
struct DummyIUnknownImpl : public IUNKNOWN
{
	virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) override {
		(*ppvObject) = this;
		return S_OK;
	}

	virtual ULONG AddRef(void) override { return 0; }
	virtual ULONG Release(void) override { return 0; }
};

}