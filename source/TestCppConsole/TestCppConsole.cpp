// TestCppConsole.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>

#include "../SusiePluginCom_i.h"
#include "../SusiePluginCom_i.c"

#if 0//def _M_IX86
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_INPROC_SERVER;
#else
constexpr auto CLCTX_SERVER_TYPE = CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER;
#endif

int main()
{
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
		if(FAILED(plugin->Load(SysAllocString(L"ifjpeg.spi"))))
		{
			printf("Failed to load SPI.\n");
		}
		else
		{
			BSTR info;
			if (SUCCEEDED(plugin->GetPluginInfo(0, &info)))
			{
				wprintf(L"Plugin Info: %ls\n", info);
				SysFreeString(info);
			}
			else
			{
				printf("Failed to get plugin info.\n");
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

