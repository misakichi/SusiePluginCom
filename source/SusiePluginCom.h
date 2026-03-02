#pragma once
#if !defined(SUSIEPLUGINCOM_H)
#define SUSIEPLUGINCOM_H

#include <Unknwn.h>

struct __declspec(uuid("90593F27-FABA-4D83-9027-D3526EF1E4BD")) IBuffer :IUnknown {
		virtual HANDLE STDMETHODCALLTYPE GetHandle() = 0;
};

//struct __declspec(uuid("08AD650D-5257-4508-93C9-1CB4839F2400")) IBuffer :IUnknown {
//	virtual HANDLE GetHandle() = 0;
//};


struct __declspec(uuid("5008EEE2-D702-4ABD-8D00-99BA90770C11")) ISusiePluginIF : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE Load(BSTR path) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetPluginInfo(int infono, BSTR* buf) = 0;
};

#endif // SUSIEPLUGINCOM_H