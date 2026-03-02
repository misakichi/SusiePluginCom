
#include <olectl.h>
#include "../SusiePluginCom_i.h"
#include <stdio.h>

static HRESULT RegisterKey(HKEY root, const wchar_t* path, const wchar_t* value)
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

HRESULT DllRegisterServer_NoInProcServer()
{
	HRESULT hr = S_OK;

	do
	{
		// AppID = library GUID
		wchar_t appGuidStr[128];

		auto ret = StringFromGUID2(LIBID_SusiePluginCom, appGuidStr, _countof(appGuidStr));
		if (ret <= 0)
		{
			hr = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
			break;
		}

		// AppID キー作成
		wchar_t appidKey[256];
		swprintf_s(appidKey, L"AppID\\%s", appGuidStr);

		if (FAILED(hr = RegisterKey(HKEY_CLASSES_ROOT, appidKey, L"SusiePluginCom")))
			break;;
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
			swprintf_s(clsidKey, 256, L"CLSID\\%s", guidStr);
			if (FAILED(hr = RegisterKey(HKEY_CLASSES_ROOT, clsidKey, L"SusiePluginCom")))
				break;;

			// CLSID に AppID を紐付け
			if (FAILED(hr = RegSetKeyValueW_HR(HKEY_CLASSES_ROOT, clsidKey, L"AppID", REG_SZ, appGuidStr, (DWORD)((wcslen(appGuidStr) + 1) * sizeof(wchar_t)))))
				break;;
		}
	} while (0);
	return hr;
}
