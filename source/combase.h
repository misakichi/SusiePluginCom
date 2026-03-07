#pragma once
#if !defined(SUSIEPLUGINCOM_COM_BASE_H)
#define SUSIEPLUGINCOM_COM_BASE_H

#include <type_traits>
#include <Unknwn.h>

/// <summary>
/// QueryInterface, AddRef, Releaseを実装するCOMオブジェクトの基底クラス
/// </summary>
/// <typeparam name="T">実装クラス</typeparam>
/// <typeparam name="I">継承インターフェース</typeparam>
template<typename I>
class ComBase : public I
{
public:
	virtual ~ComBase() {}
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override
	{
		if (CheckIID(riid))
		{
			*ppv = static_cast<I*>(this);
			AddRef();
			return S_OK;
		}
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refCount_); }
	ULONG STDMETHODCALLTYPE Release() override
	{
		auto r = InterlockedDecrement(&refCount_);
		if (r == 0)
			delete this;
		return r;
	}

private:
	static constexpr IID BaseIID = __uuidof(I);
	BOOL CheckIID(REFIID riid)
	{

		return riid == BaseIID || riid == IID_IUnknown;
	}
private:
	LONG refCount_ = 0;
};

#endif // SUSIEPLUGINCOM_COM_BASE_H