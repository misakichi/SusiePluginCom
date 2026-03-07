

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SusiePluginCom,0x852CFAED,0x6900,0x492F,0xB0,0xDE,0x05,0xEA,0x9A,0xF3,0x1E,0x68);


MIDL_DEFINE_GUID(IID, IID_ISharedMemory,0xF21BEE2A,0x8187,0x4FC8,0xA2,0x52,0x5F,0x3F,0x5F,0xBC,0xEB,0xF8);


MIDL_DEFINE_GUID(CLSID, CLSID_SharedMemory,0xCB2B781F,0x9D33,0x45DA,0x9E,0x55,0xEE,0x94,0x19,0x49,0x4B,0xFF);


MIDL_DEFINE_GUID(IID, IID_ISusie,0x35841F94,0xBF72,0x4EA7,0x92,0xC0,0x43,0x9B,0xF0,0x50,0x55,0x6D);


MIDL_DEFINE_GUID(CLSID, CLSID_SusieWrapper,0x6FD27283,0x665F,0x47F8,0xA6,0x27,0xCE,0xBF,0x40,0xC0,0xB9,0x5D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



