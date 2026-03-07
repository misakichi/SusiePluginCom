#pragma once

#include "combase.h"
#include "SusiePluginCom_i.h"  

#if defined(_M_IX86)
struct SPIFunc;
class SusiePluginIF : public ComBase<ISusie>
{
public:
	~SusiePluginIF() override;
	HRESULT STDMETHODCALLTYPE Load(BSTR path) override;
	HRESULT STDMETHODCALLTYPE GetPluginInfo(int infono, BSTR* buf) override;
	HRESULT STDMETHODCALLTYPE IsSupportedStream(BSTR filename,  IStream* stream) override;
	HRESULT STDMETHODCALLTYPE IsSupportedBuffer(BSTR filename,  SAFEARRAY* buffer) override;
	HRESULT STDMETHODCALLTYPE GetPictureInfoFile(BSTR filename, PictureInfo* info) override;
	HRESULT STDMETHODCALLTYPE GetPictureFile(BSTR filename, ISharedMemory** info, ISharedMemory** bmp) override;
	HRESULT STDMETHODCALLTYPE GetPreviewFile(BSTR filename, ISharedMemory** info, ISharedMemory** bmp) override;
	HRESULT STDMETHODCALLTYPE FinishGetPicture(void) override;

private:
	HRESULT IsSupported(BSTR filename, void* buffer);
	HRESULT PictureLocalMemToSharedMemory(HLOCAL hInfo, HLOCAL hBmp, ISharedMemory** info, ISharedMemory** bmp);
	HRESULT SpiErrorToHResult(int spiRet);

	HMODULE hModule_ = nullptr;
	SPIFunc* spiFuncs_;
	ISharedMemory* currentInfoMemory_ = nullptr;
	ISharedMemory* currentBmpMemory_ = nullptr;
};
#endif