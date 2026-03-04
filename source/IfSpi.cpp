#include "IfSpi.h"
#include <string>

#pragma pack(push,1)
struct SusiePictureInfo
{
	LONG left;
	LONG top;
	LONG width;
	LONG height;
	WORD x_density;
	WORD y_density;
	short colorDepth;
	HLOCAL info;
};
#pragma	pack(pop)
struct SPIFunc
{
	using GetPluginInfoFuncProc = int(WINAPI*)(int infono, LPSTR buf, int buflen);
	using GetPluginInfoFuncWProc = int(WINAPI*)(int infono, LPWSTR buf, int buflen);
	using IsSupportedFuncProc = int(WINAPI*)(LPCSTR filename, DWORD dw);
	using IsSupportedFuncWProc = int(WINAPI*)(LPCWSTR filename, DWORD dw);
	using GetPictureInfoFuncProc = int(WINAPI*)(LPCSTR filename, long len, DWORD flag, SusiePictureInfo* lpInfo);
	using GetPictureInfoFuncWProc = int(WINAPI*)(LPCWSTR filename, long len, DWORD flag, SusiePictureInfo* lpInfo);

	GetPluginInfoFuncProc GetPluginInfo;
	GetPluginInfoFuncWProc GetPluginInfoW;
	IsSupportedFuncProc IsSupported;
	IsSupportedFuncWProc IsSupportedW;
	GetPictureInfoFuncProc GetPictureInfo;
	GetPictureInfoFuncWProc GetPictureInfoW;
};
#define IS_FUNC_LOADED(_func, funcname)	(_func->funcname || _func->funcname ## W)
#define LOAD_FUNC_USESTR(_funcs, funcname) \
	do { \
		_funcs->funcname = (SPIFunc::funcname ## FuncProc)GetProcAddress(hModule_, #funcname);\
		if(_funcs->funcname==nullptr)\
			_funcs->funcname = (SPIFunc::funcname ## FuncProc)GetProcAddress(hModule_, #funcname "A");\
		_funcs->funcname ## W = (SPIFunc::funcname ## FuncWProc)GetProcAddress(hModule_, #funcname "W");\
	} while(0); \
	if (IS_FUNC_LOADED(spiFuncs_, GetPluginInfo) == false) \
	 break

static std::string bstrToStr(BSTR bstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, bstr, -1, nullptr, 0, nullptr, nullptr);
	std::string str(len - 1, '\0');
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, &str[0], len, nullptr, nullptr);
	return str;
}
static BSTR strToBstr(const char* str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
	std::wstring wstr(len - 1, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], len);
	return SysAllocString(wstr.c_str());
}

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
		LOAD_FUNC_USESTR(spiFuncs_, IsSupported);
		LOAD_FUNC_USESTR(spiFuncs_, GetPictureInfo);
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

/// <summary>
/// GetPluginInfo
/// infono 0: プラグインの名前
/// infono 1: プラグインのバージョン
/// infono 2 + n * 2 : 対応拡張子
/// infono 3 + n * 2 : ファイル種別名
/// </summary>
/// <param name="infono">情報番号</param>
/// <param name="buf">戻りのBSTR</param>
/// <returns></returns>
HRESULT SusiePluginIF::GetPluginInfo(int infono, BSTR* buf)
{
	if(hModule_==nullptr)
		return CO_E_NOTINITIALIZED;

	if (spiFuncs_->GetPluginInfo)
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
	else if (spiFuncs_->GetPluginInfoW)
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
	else
		return E_NOTIMPL;
}

constexpr int IsSupportedRequiredBufferSize = 2048;
/// <summary>
/// IStream版IsSupported実装
/// </summary>
/// <param name="filename"></param>
/// <param name="stream"></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SusiePluginIF::IsSupportedStream(BSTR filename, IStream* stream)
{
	BYTE buffer[IsSupportedRequiredBufferSize] = {};
	ULONG readBytes = 0;
	HRESULT hr;
	if (FAILED(hr = stream->Seek({ 0 }, STREAM_SEEK_SET, nullptr)))
		return hr;
	if(FAILED(hr=stream->Read(buffer, IsSupportedRequiredBufferSize, &readBytes)))
		return hr;
	return IsSupported(filename, buffer);
}

/// <summary>
/// メモリ版IsSupported実装
/// </summary>
/// <param name="filename"></param>
/// <param name="buffer"></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SusiePluginIF::IsSupportedBuffer(BSTR filename, SAFEARRAY* buffer)
{
	LONG lbound = 0, ubound = 0;
	SafeArrayGetLBound(buffer, 1, &lbound);
	SafeArrayGetUBound(buffer, 1, &ubound);
	LONG elementNum = ubound - lbound + 1;
	auto size = SafeArrayGetElemsize(buffer) * elementNum;
	if (size < IsSupportedRequiredBufferSize)
	{
		return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
	}
	void* pData;
	SafeArrayAccessData(buffer, &pData);
	auto ret = IsSupported(filename, pData);
	SafeArrayUnaccessData(buffer);
	return ret;
}

HRESULT SusiePluginIF::IsSupported(BSTR filename, void* buffer)
{
	if (spiFuncs_->IsSupported)
	{
		std::string str = bstrToStr(filename);
		return spiFuncs_->IsSupported(str.c_str(), (DWORD)buffer) ? S_OK : S_FALSE;

	}
	else if (spiFuncs_->IsSupportedW)
	{
		return spiFuncs_->IsSupportedW(filename, (DWORD)buffer) ? S_OK : S_FALSE;
	}
	else
	{
		return E_NOTIMPL;
	}
}

/// <summary>
/// File版GetPictureInfoFile実装
/// </summary>
/// <param name="filename"></param>
/// <param name="info"></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SusiePluginIF::GetPictureInfoFile(BSTR filename, PictureInfo* info)
{
	if (hModule_ == nullptr)
		return CO_E_NOTINITIALIZED;
	
	SusiePictureInfo spiInfo = {};
	int ret = 0;
	if (spiFuncs_->GetPluginInfo)
	{
		std::string str = bstrToStr(filename);
		ret = spiFuncs_->GetPictureInfo(str.c_str(), 0, 0, &spiInfo);
	}
	else if (spiFuncs_->GetPluginInfoW)
	{
		ret = spiFuncs_->GetPictureInfoW(filename, 0, 0, &spiInfo);
	}
	else
		return E_NOTIMPL;

	info->left = spiInfo.left;
	info->top = spiInfo.top;
	info->width = spiInfo.width;
	info->height = spiInfo.height;
	info->x_density = spiInfo.x_density;
	info->y_density = spiInfo.y_density;
	info->colorDepth = spiInfo.colorDepth;
	info->pd = 0;
	if (spiInfo.info)
	{
		if (auto mem = (const char*)LocalLock(spiInfo.info))
		{
			info->info = strToBstr(mem);
			LocalUnlock(spiInfo.info);
			LocalFree(spiInfo.info);
		}
	}
	else
	{
		info->info = nullptr;
	}
	return ret==0 ? S_OK : HRESULT_FROM_WIN32(ret);
}
