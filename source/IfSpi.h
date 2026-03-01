#pragma once

#include "SusiePluginCom.h"
#include "combase.h"
#include "SusiePluginCom_i.h"  

struct SPIFunc;
class SusiePluginIF : public ComBase<ISusiePluginIF, ISusie>
{
public:
    HRESULT STDMETHODCALLTYPE Load(BSTR path) override;
    HRESULT STDMETHODCALLTYPE GetPluginInfo(int infono, BSTR* buf) override;

private:
    HMODULE hModule_ = nullptr;
    SPIFunc* spiFuncs_;
};
