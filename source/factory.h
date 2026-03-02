#pragma once
#if !defined(_SUSIEPLUGINCOM_FACTORY_H)
#define _SUSIEPLUGINCOM_FACTORY_H

#include <Unknwn.h>
#include "combase.h"

template<typename T> 
class SusiePluginComGenericFactory : public ComBase<IClassFactory, void>
{
public:
	HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* outer, REFIID riid, void** ppv) override
	{
		if (outer)
			return CLASS_E_NOAGGREGATION;
		auto obj = new T();
		return obj->QueryInterface(riid, ppv);
	}


	HRESULT STDMETHODCALLTYPE LockServer(BOOL) override { return S_OK; }
};



#endif // _SUSIEPLUGINCOM_FACTORY_H
