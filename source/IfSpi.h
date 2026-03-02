#pragma once

#include "combase.h"
#include "SusiePluginCom_i.h"  

struct SPIFunc;
class SusiePluginIF : public ComBase<ISusie>
{
public:
	HRESULT STDMETHODCALLTYPE Load(BSTR path) override;
	HRESULT STDMETHODCALLTYPE GetPluginInfo(int infono, BSTR* buf) override;
	HRESULT STDMETHODCALLTYPE IsSupportedStream(BSTR filename,  IStream* stream)override;
	HRESULT STDMETHODCALLTYPE IsSupportedBuffer(BSTR filename,  SAFEARRAY* buffer)override;

private:
	HRESULT IsSupported(BSTR filename, void* buffer);

	HMODULE hModule_ = nullptr;
	SPIFunc* spiFuncs_;
};
