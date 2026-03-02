// TestCppConsole.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>
#include "olectl.h"

#include "../SusiePluginCom_i.h"
#include "../SusiePluginCom_i.c"

#include <atlbase.h>

#if 0//def _M_IX86
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_INPROC_SERVER;
#else
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER;
#endif

HRESULT DllRegisterServer_NoInProcServer();

static CComPtr<IStream> CreateStreamFromFile(const wchar_t* filePath)
{
	CComPtr<IStream> stream;
	HRESULT hr = SHCreateStreamOnFileW(filePath, STGM_READ, &stream);
	return stream;
}
static SAFEARRAY* LoadFileToSafeArray(const wchar_t* filePath, size_t bufferSize)
{
	SAFEARRAYBOUND bound = { bufferSize, 0 };
	auto array = SafeArrayCreate(VT_UI1, 1, &bound);
	BYTE* pData;
	SafeArrayAccessData(array, (void**)&pData);
	memset(pData, 0, bufferSize);
	FILE* file;
	_wfopen_s(&file, filePath, L"rb");
	if(file)
	{
		fread(pData, 1, bufferSize, file);
		fclose(file);
	}
	SafeArrayUnaccessData(array);
	return array;
}

int main()
{
	if(FAILED(DllRegisterServer_NoInProcServer()))
	{
		printf("Failed to register COM server.\n");
	}
	if (FAILED(CoInitialize(nullptr)))
	{
		printf("Failed to initialize COM library.\n");
		return 1;
	}	

	ISusie* plugin = nullptr;
	
	auto hr = CoCreateInstance(CLSID_SusieWrapper, nullptr, CLCTX_SERVER_TYPE, IID_PPV_ARGS(&plugin));
	if(SUCCEEDED(hr) && plugin)
	{
		printf("Plugin interface create successfully.\n");
		if(FAILED(hr = plugin->Load(SysAllocString(L"C:\\projects\\SusiePluginCom\\source\\Debug\\ifjpeg.spi"))))
		{
			printf("Failed to load SPI.\n");
		}
		else
		{
			BSTR info;
			int infono = 0;
			while (1)
			{
				if (SUCCEEDED(plugin->GetPluginInfo(infono, &info)))
				{
					wprintf(L"Plugin Info(%d): %ls\n", infono, info);
					SysFreeString(info);
				}
				else
				{
					printf("Failed to get plugin info(%d).\n", infono);
					break;
				}
				infono++;
			}

			const wchar_t* testFilePath[] =
			{
				L"C:\\projects\\SusiePluginCom\\source\\Debug\\DSC_0696.jpg",
				L"C:\\projects\\SusiePluginCom\\source\\Debug\\DSC_0697.jpg",
				L"C:\\projects\\SusiePluginCom\\source\\Debug\\DSC_0698.jpg",
				L"C:\\projects\\SusiePluginCom\\source\\Debug\\pngnow.png",
			};
			wprintf(L"IsSupportedStream test(Stream):\n");
			for (auto checkPath : testFilePath)
			{
				auto bstrPath = SysAllocString(checkPath);
				auto stream = CreateStreamFromFile(checkPath);
				if (SUCCEEDED(hr = plugin->IsSupportedStream(bstrPath, stream)))
				{
					wprintf(L"IsSupportedStream: %ls is %s.\n", checkPath, hr == S_OK ? L"supported" : L"not supported");
				}
				else
				{
					wprintf(L"IsSupportedStream: %ls error.\n", checkPath);
				}
				SysFreeString(bstrPath);
			}
			wprintf(L"IsSupportedStream test(array):\n");
			for (auto checkPath : testFilePath)
			{
				auto bstrPath = SysAllocString(checkPath);
				auto array = LoadFileToSafeArray(checkPath, 2048);
				if (SUCCEEDED(hr = plugin->IsSupportedBuffer(bstrPath, array)))
				{
					wprintf(L"IsSupportedStream: %ls is %s.\n", checkPath, hr == S_OK ? L"supported" : L"not supported");
				}
				else
				{
					wprintf(L"IsSupportedStream: %ls error.\n", checkPath);
				}
				SysFreeString(bstrPath);
				SafeArrayDestroy(array);
			}


		}
		plugin->Release();
	}
	else
	{
		printf("Failed to create plugin interface.\n");
	}
	
	CoUninitialize();
}

