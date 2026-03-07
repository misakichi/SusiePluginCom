

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for SusiePluginCom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __SusiePluginCom_i32_h__
#define __SusiePluginCom_i32_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __ISharedMemory_FWD_DEFINED__
#define __ISharedMemory_FWD_DEFINED__
typedef interface ISharedMemory ISharedMemory;

#endif 	/* __ISharedMemory_FWD_DEFINED__ */


#ifndef __SharedMemory_FWD_DEFINED__
#define __SharedMemory_FWD_DEFINED__

#ifdef __cplusplus
typedef class SharedMemory SharedMemory;
#else
typedef struct SharedMemory SharedMemory;
#endif /* __cplusplus */

#endif 	/* __SharedMemory_FWD_DEFINED__ */


#ifndef __ISusie_FWD_DEFINED__
#define __ISusie_FWD_DEFINED__
typedef interface ISusie ISusie;

#endif 	/* __ISusie_FWD_DEFINED__ */


#ifndef __SusieWrapper_FWD_DEFINED__
#define __SusieWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class SusieWrapper SusieWrapper;
#else
typedef struct SusieWrapper SusieWrapper;
#endif /* __cplusplus */

#endif 	/* __SusieWrapper_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SusiePluginCom_LIBRARY_DEFINED__
#define __SusiePluginCom_LIBRARY_DEFINED__

/* library SusiePluginCom */
/* [version][uuid] */ 

typedef struct _PictureInfo
    {
    __int32 left;
    __int32 top;
    __int32 width;
    __int32 height;
    WORD x_density;
    WORD y_density;
    short colorDepth;
    short pd;
    BSTR info;
    } 	PictureInfo;


EXTERN_C const IID LIBID_SusiePluginCom;

#ifndef __ISharedMemory_INTERFACE_DEFINED__
#define __ISharedMemory_INTERFACE_DEFINED__

/* interface ISharedMemory */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ISharedMemory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F21BEE2A-8187-4FC8-A252-5F3F5FBCEBF8")
    ISharedMemory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateBuffer( 
            /* [in] */ DWORD size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBuffer( 
            /* [out] */ BYTE **buffer,
            /* [out] */ DWORD *size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Lock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unlock( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISharedMemoryVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISharedMemory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISharedMemory * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISharedMemory * This);
        
        DECLSPEC_XFGVIRT(ISharedMemory, CreateBuffer)
        HRESULT ( STDMETHODCALLTYPE *CreateBuffer )( 
            ISharedMemory * This,
            /* [in] */ DWORD size);
        
        DECLSPEC_XFGVIRT(ISharedMemory, GetBuffer)
        HRESULT ( STDMETHODCALLTYPE *GetBuffer )( 
            ISharedMemory * This,
            /* [out] */ BYTE **buffer,
            /* [out] */ DWORD *size);
        
        DECLSPEC_XFGVIRT(ISharedMemory, Lock)
        HRESULT ( STDMETHODCALLTYPE *Lock )( 
            ISharedMemory * This);
        
        DECLSPEC_XFGVIRT(ISharedMemory, Unlock)
        HRESULT ( STDMETHODCALLTYPE *Unlock )( 
            ISharedMemory * This);
        
        END_INTERFACE
    } ISharedMemoryVtbl;

    interface ISharedMemory
    {
        CONST_VTBL struct ISharedMemoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISharedMemory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISharedMemory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISharedMemory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISharedMemory_CreateBuffer(This,size)	\
    ( (This)->lpVtbl -> CreateBuffer(This,size) ) 

#define ISharedMemory_GetBuffer(This,buffer,size)	\
    ( (This)->lpVtbl -> GetBuffer(This,buffer,size) ) 

#define ISharedMemory_Lock(This)	\
    ( (This)->lpVtbl -> Lock(This) ) 

#define ISharedMemory_Unlock(This)	\
    ( (This)->lpVtbl -> Unlock(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISharedMemory_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SharedMemory;

#ifdef __cplusplus

class DECLSPEC_UUID("CB2B781F-9D33-45DA-9E55-EE9419494BFF")
SharedMemory;
#endif

#ifndef __ISusie_INTERFACE_DEFINED__
#define __ISusie_INTERFACE_DEFINED__

/* interface ISusie */
/* [oleautomation][unique][uuid][object] */ 


EXTERN_C const IID IID_ISusie;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("35841F94-BF72-4EA7-92C0-439BF050556D")
    ISusie : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR path) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPluginInfo( 
            /* [in] */ int infono,
            /* [retval][out] */ BSTR *buf) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsSupportedStream( 
            /* [in] */ BSTR filename,
            /* [in] */ IStream *stream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsSupportedBuffer( 
            /* [in] */ BSTR filename,
            /* [in] */ SAFEARRAY * buffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPictureInfoFile( 
            /* [in] */ BSTR filename,
            /* [out] */ PictureInfo *info) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPictureFile( 
            /* [in] */ BSTR filename,
            /* [out] */ ISharedMemory **info,
            /* [out] */ ISharedMemory **bmp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPreviewFile( 
            /* [in] */ BSTR filename,
            /* [out] */ ISharedMemory **info,
            /* [out] */ ISharedMemory **bmp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FinishGetPicture( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ISusieVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISusie * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISusie * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISusie * This);
        
        DECLSPEC_XFGVIRT(ISusie, Load)
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            ISusie * This,
            /* [in] */ BSTR path);
        
        DECLSPEC_XFGVIRT(ISusie, GetPluginInfo)
        HRESULT ( STDMETHODCALLTYPE *GetPluginInfo )( 
            ISusie * This,
            /* [in] */ int infono,
            /* [retval][out] */ BSTR *buf);
        
        DECLSPEC_XFGVIRT(ISusie, IsSupportedStream)
        HRESULT ( STDMETHODCALLTYPE *IsSupportedStream )( 
            ISusie * This,
            /* [in] */ BSTR filename,
            /* [in] */ IStream *stream);
        
        DECLSPEC_XFGVIRT(ISusie, IsSupportedBuffer)
        HRESULT ( STDMETHODCALLTYPE *IsSupportedBuffer )( 
            ISusie * This,
            /* [in] */ BSTR filename,
            /* [in] */ SAFEARRAY * buffer);
        
        DECLSPEC_XFGVIRT(ISusie, GetPictureInfoFile)
        HRESULT ( STDMETHODCALLTYPE *GetPictureInfoFile )( 
            ISusie * This,
            /* [in] */ BSTR filename,
            /* [out] */ PictureInfo *info);
        
        DECLSPEC_XFGVIRT(ISusie, GetPictureFile)
        HRESULT ( STDMETHODCALLTYPE *GetPictureFile )( 
            ISusie * This,
            /* [in] */ BSTR filename,
            /* [out] */ ISharedMemory **info,
            /* [out] */ ISharedMemory **bmp);
        
        DECLSPEC_XFGVIRT(ISusie, GetPreviewFile)
        HRESULT ( STDMETHODCALLTYPE *GetPreviewFile )( 
            ISusie * This,
            /* [in] */ BSTR filename,
            /* [out] */ ISharedMemory **info,
            /* [out] */ ISharedMemory **bmp);
        
        DECLSPEC_XFGVIRT(ISusie, FinishGetPicture)
        HRESULT ( STDMETHODCALLTYPE *FinishGetPicture )( 
            ISusie * This);
        
        END_INTERFACE
    } ISusieVtbl;

    interface ISusie
    {
        CONST_VTBL struct ISusieVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISusie_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISusie_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISusie_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISusie_Load(This,path)	\
    ( (This)->lpVtbl -> Load(This,path) ) 

#define ISusie_GetPluginInfo(This,infono,buf)	\
    ( (This)->lpVtbl -> GetPluginInfo(This,infono,buf) ) 

#define ISusie_IsSupportedStream(This,filename,stream)	\
    ( (This)->lpVtbl -> IsSupportedStream(This,filename,stream) ) 

#define ISusie_IsSupportedBuffer(This,filename,buffer)	\
    ( (This)->lpVtbl -> IsSupportedBuffer(This,filename,buffer) ) 

#define ISusie_GetPictureInfoFile(This,filename,info)	\
    ( (This)->lpVtbl -> GetPictureInfoFile(This,filename,info) ) 

#define ISusie_GetPictureFile(This,filename,info,bmp)	\
    ( (This)->lpVtbl -> GetPictureFile(This,filename,info,bmp) ) 

#define ISusie_GetPreviewFile(This,filename,info,bmp)	\
    ( (This)->lpVtbl -> GetPreviewFile(This,filename,info,bmp) ) 

#define ISusie_FinishGetPicture(This)	\
    ( (This)->lpVtbl -> FinishGetPicture(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISusie_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SusieWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("6FD27283-665F-47F8-A627-CEBF40C0B95D")
SusieWrapper;
#endif
#endif /* __SusiePluginCom_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


