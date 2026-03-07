#include "SharedMemory.h"

#include <string>

SharedMemory::SharedMemory()
{
}
SharedMemory::~SharedMemory()
{
	Destroy();
}

void SharedMemory::Destroy()
{
	if (addr_)
		UnmapViewOfFile(addr_);
	if (mapFile_)
		delete mapFile_;
	if (mutex_)
		delete mutex_;
	addr_ = nullptr;
	mapFile_ = nullptr;
	mutex_ = nullptr;
}

SharedMemory::NamedOsObject::~NamedOsObject()
{
	if (handle)
		CloseHandle(handle);
}

/// <summary>
/// 名前付きオブジェクトの作成
/// </summary>
/// <param name="prefix"></param>
/// <param name="idGenerater"></param>
/// <param name="createFunc"></param>
/// <param name="object"></param>
/// <returns></returns>
HRESULT SharedMemory::CreateNamedObject(const char* prefix, uint32_t& idGenerater, std::function<HANDLE(const char*)> createFunc, NamedOsObject** object)
{
	if (object == nullptr)
		return E_POINTER;
	
	uint32_t id;
	HANDLE handle;
	while (1)
	{
		id = InterlockedIncrement(&idGenerater);
		auto name = prefix + std::to_string(id);
		handle = createFunc(name.c_str());
		if (handle == nullptr)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(handle);
			continue;
		}
		break;
	}
	*object = new NamedOsObject{ handle, id };
	return S_OK;
}


// ISharedMemory の実装
HRESULT STDMETHODCALLTYPE SharedMemory::CreateBuffer(DWORD size) 
{

	static uint32_t s_FileID = 0;
	static uint32_t s_MutexID = 0;

	NamedOsObject* mapFileObject = nullptr;
	NamedOsObject* mutexObject = nullptr;
	void* mappedAddr = nullptr;
	HRESULT hr = S_OK;
	do
	{
		if(FAILED(hr=CreateNamedObject(MappedFileGUID, s_FileID, [size](const char* name) {
			return CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size, name);
			}, & mapFileObject)))
			break;
		if (FAILED(hr = CreateNamedObject(MutexGUID, s_MutexID, [](const char* name) {
				return CreateMutexA(NULL, FALSE, name);
			}, &mutexObject)))
			break;
		
		mappedAddr = MapViewOfFile(mapFileObject->handle, FILE_MAP_ALL_ACCESS, 0, 0, size);
		if (mappedAddr == nullptr)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		}

	} while (0);

	if(FAILED(hr))
	{
		if(mappedAddr)
			UnmapViewOfFile(mappedAddr);
		if (mapFileObject)
			delete mapFileObject;
		if (mutexObject)
			delete mutexObject;
		return hr;
	}



	addr_ = mappedAddr;
	size_ = size;

	mapFile_ = mapFileObject;
	mutex_ = mutexObject;


	return S_OK;
}

/// <summary>
/// 確保している共有メモリのアドレスとサイズを取得する
/// </summary>
/// <param name="buffer"></param>
/// <param name="size"></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SharedMemory::GetBuffer(BYTE** buffer, DWORD* size)
{
	if(addr_)
	{
		*buffer = (BYTE*)addr_;
		*size = size_;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

/// <summary>
/// 同期処理
/// </summary>
/// <param name=""></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SharedMemory::Lock(void) 
{
	if(mutex_==nullptr || mutex_->handle==nullptr)
		return E_FAIL;
	return HRESULT_FROM_WIN32(WaitForSingleObject(mutex_->handle, INFINITE));
}

/// <summary>
/// 同期処理
/// </summary>
/// <param name=""></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SharedMemory::Unlock(void)
{
	if (mutex_ == nullptr || mutex_->handle == nullptr)
		return E_FAIL;
	return HRESULT_FROM_WIN32(ReleaseMutex(mutex_->handle));
}

// IMarshal の実装

/// <summary>
/// 
/// </summary>
/// <param name="riid"></param>
/// <param name="pv"></param>
/// <param name="dwDestContext"></param>
/// <param name="pvDestContext"></param>
/// <param name="mshlflags"></param>
/// <param name="pCid"></param>
/// <returns></returns>
HRESULT STDMETHODCALLTYPE SharedMemory::GetUnmarshalClass(REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, CLSID* pCid)
{
	*pCid = CLSID_SharedMemory;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SharedMemory::GetMarshalSizeMax(REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags, DWORD* pSize) 
{
	*pSize = sizeof(DWORD) * 6;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SharedMemory::MarshalInterface(IStream* pStm, REFIID riid, void* pv, DWORD dwDestContext, void* pvDestContext, DWORD mshlflags)
{

	if (addr_ == nullptr)
		return E_FAIL;

	pStm->Write(&size_, sizeof(DWORD), nullptr);
	pStm->Write(&mapFile_->id, sizeof(DWORD), nullptr);
	pStm->Write(&mutex_->id, sizeof(DWORD), nullptr);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SharedMemory::UnmarshalInterface(IStream* pStm, REFIID riid, void** ppv) 
{
	DWORD size, mapFileId, mutexId;
	pStm->Read(&size, sizeof(DWORD), nullptr);
	pStm->Read(&mapFileId, sizeof(DWORD), nullptr);
	pStm->Read(&mutexId, sizeof(DWORD), nullptr);

	HANDLE hMapFile = nullptr;
	HANDLE hMutex = nullptr;
	void* addr = nullptr;
	// 受け取ったIDを元にハンドルを開く
	HRESULT hr = S_OK;
	do
	{
		hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, (MappedFileGUID + std::to_string(mapFileId)).c_str());
		if (hMapFile == nullptr)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		}
		hMutex = OpenMutexA(SYNCHRONIZE, FALSE, (MutexGUID + std::to_string(mutexId)).c_str());
		if (hMutex == nullptr)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		}
		addr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
		if (addr == nullptr)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		}

	} while (0);

	if (FAILED(hr))
	{
		if(addr)
			UnmapViewOfFile(addr);
		if(hMapFile)
			CloseHandle(hMapFile);
		if (hMutex)
			CloseHandle(hMutex);
		return hr;
	}

	mapFile_ = new NamedOsObject{ hMapFile, mapFileId };
	mutex_ = new NamedOsObject{ hMutex, mutexId };
	addr_ = addr;
	size_ = size;

	return ComBase<ISharedMemory>::QueryInterface(riid,ppv);
}
HRESULT STDMETHODCALLTYPE SharedMemory::ReleaseMarshalData(IStream* pStm) 
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE SharedMemory::DisconnectObject(DWORD dwReserved) 
{
	return S_OK;
}
