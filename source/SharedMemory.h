#pragma once
#include "combase.h"
#include "SusiePluginCom_i.h"  

#include <functional>

class SharedMemory : public ComBase<ISharedMemory>, public IMarshal
{
public:
	SharedMemory();
	virtual ~SharedMemory();

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override
	{		
		if (riid == __uuidof(IMarshal))
		{
			*ppv = static_cast<IMarshal*>(this);
			AddRef();
			return S_OK;
		}
		return ComBase<ISharedMemory>::QueryInterface(riid, ppv);
	}
	ULONG STDMETHODCALLTYPE AddRef() override { return ComBase<ISharedMemory>::AddRef();; }
	ULONG STDMETHODCALLTYPE Release() override { return ComBase<ISharedMemory>::Release(); }


	// ISharedMemory の実装
	HRESULT STDMETHODCALLTYPE CreateBuffer(DWORD size) override;
	HRESULT STDMETHODCALLTYPE GetBuffer(BYTE** buffer, DWORD* size) override;
	HRESULT STDMETHODCALLTYPE Lock(void) override;
	HRESULT STDMETHODCALLTYPE Unlock(void) override;
	HANDLE STDMETHODCALLTYPE GetHandle() override { return mapFile_ ? mapFile_->handle : NULL; }
	HRESULT STDMETHODCALLTYPE GetPathName(BSTR* path) override;

	// IMarshal の実装
	HRESULT STDMETHODCALLTYPE GetUnmarshalClass(REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, CLSID* pCid) override;
	HRESULT STDMETHODCALLTYPE GetMarshalSizeMax(REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, DWORD* pSize) override;
	HRESULT STDMETHODCALLTYPE MarshalInterface(IStream* pStm, REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags) override;
	HRESULT STDMETHODCALLTYPE UnmarshalInterface(IStream* pStm, REFIID riid, void** ppv) override;
	HRESULT STDMETHODCALLTYPE ReleaseMarshalData(IStream* pStm) override;
	HRESULT STDMETHODCALLTYPE DisconnectObject(DWORD dwReserved) override;

private:
	class NamedOsObject
	{
	public:
		~NamedOsObject();
		HANDLE handle;
		uint32_t id;
	};

	void Destroy();
	HRESULT CreateNamedObject(const wchar_t* prefix, uint32_t& idGenerater, std::function<HANDLE(const wchar_t*)> createFunc, NamedOsObject** object);

	NamedOsObject* mapFile_ = nullptr;
	NamedOsObject* mutex_ = nullptr;

	static constexpr auto MappedFileGUID = L"Global\\{847AE6E8-B219-469A-8E3A-398630F4D3A5}";
	static constexpr auto MutexGUID = L"Global\\{46E8D01A-982A-4EAE-966F-B2A62988B8E3}";
	static constexpr auto MarshalSyncGUID = L"Global\\{AACC91E5-B1DB-4A8A-98BB-943B526E09AF}";

	void* addr_ = nullptr;
	DWORD size_ = 0;

};

