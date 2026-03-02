#include "factory.h"
#include "IfSpi.h"
#include "SusiePluginCom_i.h"
#include "olectl.h"
#include <strsafe.h>

HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv) 
{

	if (clsid == CLSID_SusieWrapper) {
		static SusiePluginComGenericFactory<SusiePluginIF> factory;
		return factory.QueryInterface(riid, ppv);
	}
	return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT __stdcall DllCanUnloadNow() 
{
	return S_FALSE; // 単純化のため常にアンロード不可
}


extern "C" IMAGE_DOS_HEADER __ImageBase;

// レジストリキー作成ヘルパー
HRESULT RegisterKey(HKEY root, const wchar_t* path, const wchar_t* value)
{
	HKEY hKey;
	if (RegCreateKeyExW(root, path, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) != ERROR_SUCCESS)
		return SELFREG_E_CLASS;

	if (value)
		RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)value, (DWORD)((wcslen(value) + 1) * sizeof(wchar_t)));

	RegCloseKey(hKey);
	return S_OK;
}

// 登録する CLSID 群
static const GUID s_CLSIDs[] =
{
	CLSID_SusieWrapper,
};

HRESULT RegSetKeyValueW_HR(
	HKEY hKey,
	LPCWSTR lpSubKey,
	LPCWSTR lpValueName,
	DWORD dwType,
	LPCVOID lpData,
	DWORD cbData
)
{
	return HRESULT_FROM_WIN32(RegSetKeyValueW(hKey, lpSubKey, lpValueName, dwType, lpData, cbData));
}

extern "C" HRESULT __stdcall DllRegisterServer()
{
	HRESULT hr = S_OK;

	do
	{
		// AppID = library GUID
		wchar_t appGuidStr[128];

		auto ret= StringFromGUID2(LIBID_SusiePluginCom, appGuidStr, _countof(appGuidStr));
		if (ret <= 0)
		{
			hr = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
			break;
		}

		// DLL 自身のパス
		wchar_t modulePath[MAX_PATH];
		if (GetModuleFileNameW((HINSTANCE)&__ImageBase, modulePath, MAX_PATH) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		}

		// AppID キー作成
		wchar_t appidKey[256];
		StringCchPrintfW(appidKey, 256, L"AppID\\%s", appGuidStr);


		if (FAILED(hr = RegisterKey(HKEY_CLASSES_ROOT, appidKey, L"SusiePluginCom")))
			break;
		RegSetKeyValueW_HR(HKEY_CLASSES_ROOT, appidKey, L"DllSurrogate", REG_SZ, L"", sizeof(wchar_t));

		// CLSID ごとに登録
		for (auto& clsid : s_CLSIDs)
		{
			wchar_t guidStr[128];
			auto ret = StringFromGUID2(clsid, guidStr, _countof(guidStr));
			if (ret <= 0)
			{
				hr = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
				break;
			}

			// CLSID\{GUID}
			wchar_t clsidKey[256];
			StringCchPrintfW(clsidKey, 256, L"CLSID\\%s", guidStr);
			if (FAILED(hr = RegisterKey(HKEY_CLASSES_ROOT, clsidKey, L"SusiePluginCom")))
				break;

			// CLSID\{GUID}\InprocServer32
			wchar_t inprocKey[256];
			StringCchPrintfW(inprocKey, 256, L"%s\\InprocServer32", clsidKey);

			if (FAILED(hr = RegisterKey(HKEY_CLASSES_ROOT, inprocKey, modulePath)))
				break;
			if (FAILED(hr = RegSetKeyValueW_HR(HKEY_CLASSES_ROOT, inprocKey, L"ThreadingModel", REG_SZ, L"Both", sizeof(L"Both"))))
				break;

			// CLSID に AppID を紐付け
			if (FAILED(hr = RegSetKeyValueW_HR(HKEY_CLASSES_ROOT, clsidKey, L"AppID", REG_SZ, appGuidStr, (DWORD)((wcslen(appGuidStr) + 1) * sizeof(wchar_t)))))
				break;

		}
		wchar_t* lastDirSplt = modulePath;
		auto search = modulePath;
		while (search = wcschr(search, L'\\'))
		{
			search++;
			lastDirSplt = search;
		}
		wcscpy_s(lastDirSplt, MAX_PATH - (lastDirSplt - modulePath), L"SusiePluginCom.tlb");
		ITypeLib* pTypeLib = nullptr;
		if (FAILED(hr = LoadTypeLibEx(modulePath, REGKIND_REGISTER, &pTypeLib)))
			break;
		if (pTypeLib) 
			pTypeLib->Release();
	} while (0);

	if(FAILED(hr))
		DllUnregisterServer();

	return hr;
}

extern "C" HRESULT __stdcall DllUnregisterServer()
{
	HRESULT hr = S_OK;
	HRESULT hrRet = S_OK;
	wchar_t guidStr[64];
	// CLSID ごとに登録
	for (auto& clsid : s_CLSIDs)
	{
		wchar_t clsidKey[256];
		if (StringFromGUID2(clsid, guidStr, 64) == 0)
			hrRet = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
		if(FAILED(hr=StringCchPrintfW(clsidKey, 256, L"CLSID\\%s", guidStr)))
			hrRet = hr;
		if(FAILED(hr=HRESULT_FROM_WIN32(RegDeleteTreeW(HKEY_CLASSES_ROOT, clsidKey))))
			hrRet = hr;
	}

	if (StringFromGUID2(LIBID_SusiePluginCom, guidStr, 64) == 0)
		hrRet = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);

	wchar_t appidKey[256];
	if(FAILED(hr=StringCchPrintfW(appidKey, 256, L"AppID\\%s", guidStr)))
		hrRet = hr;
	if(FAILED(hr=HRESULT_FROM_WIN32(RegDeleteTreeW(HKEY_CLASSES_ROOT, appidKey))))
		hrRet = hr;

	if(FAILED(hr=UnRegisterTypeLib(LIBID_SusiePluginCom, 1, 0, LOCALE_NEUTRAL, SYS_WIN32 )))
		hrRet = hr;

	return hrRet;
}