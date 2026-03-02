// TestCppConsole.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>
#include "olectl.h"

#include "../SusiePluginCom_i.h"
#include "../SusiePluginCom_i.c"

#if 0//def _M_IX86
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_INPROC_SERVER;
#else
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER;
#endif

HRESULT DllRegisterServer_NoInProcServer();

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
		}
		plugin->Release();
	}
	else
	{
		printf("Failed to create plugin interface.\n");
	}
	
	CoUninitialize();
}

