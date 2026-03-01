#include "IfSpi.h"

struct SPIFunc
{
	using GetPluginInfoFuncProc = int(WINAPI*)(int infono, LPSTR buf, int buflen);
	using GetPluginInfoFuncWProc = int(WINAPI*)(int infono, LPWSTR buf, int buflen);

	GetPluginInfoFuncProc GetPluginInfo;
	GetPluginInfoFuncWProc GetPluginInfoW;
};
#define IS_FUNC_LOADED(_func, funcname)	(_func->funcname || _func->funcname ## W)
#define LOAD_FUNC_USESTR(_funcs, funcname) \
	do { \
		_funcs->funcname = (SPIFunc::funcname ## FuncProc)GetProcAddress(hModule_, #funcname);\
		_funcs->funcname ## W = (SPIFunc::funcname ## FuncWProc)GetProcAddress(hModule_, #funcname "W");\
	} while(0); \
	if (IS_FUNC_LOADED(spiFuncs_, GetPluginInfo) == false) \
	 break

HRESULT SusiePluginIF::Load(BSTR path) 
{
	hModule_ = LoadLibraryW(path);
	if (hModule_==nullptr)
		return HRESULT_FROM_WIN32(GetLastError());

	bool funcloadComplete = false;
	spiFuncs_ = new SPIFunc;
	do
	{
		LOAD_FUNC_USESTR(spiFuncs_, GetPluginInfo);
		funcloadComplete = true;
	} while (0);

	if (funcloadComplete == false)
	{
		delete spiFuncs_;
		spiFuncs_ = nullptr;
		FreeLibrary(hModule_);
		hModule_ = nullptr;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SusiePluginIF::GetPluginInfo(int infono, BSTR* buf)
{
	if(hModule_==nullptr)
		return CO_E_NOTINITIALIZED;

	if (spiFuncs_->GetPluginInfoW)
	{
		wchar_t tmpBuf[256];
		auto wrote = spiFuncs_->GetPluginInfoW(infono, tmpBuf, _countof(tmpBuf));
		if (wrote > 0)
		{
			
			*buf = SysAllocString(tmpBuf);
			return S_OK;
		}
		else
			return E_FAIL;
	}
	else if (spiFuncs_->GetPluginInfo)
	{
		char tmpBuf[256];
		auto wrote = (*spiFuncs_->GetPluginInfo)(infono, &tmpBuf[0], _countof(tmpBuf));
		if (wrote > 0)
		{
			wchar_t wTmpBuf[256];
			MultiByteToWideChar(CP_ACP, 0, tmpBuf, -1, wTmpBuf, _countof(wTmpBuf));
			*buf = SysAllocString(wTmpBuf);
			return S_OK;
		}
		else
			return E_FAIL;
	}
	else
		return E_NOTIMPL;
}

