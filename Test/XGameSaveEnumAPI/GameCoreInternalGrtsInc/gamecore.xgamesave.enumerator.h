

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __gamesaveenumerator_h__
#define __gamesaveenumerator_h__

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

#ifndef __IXGameSaveProviderEnumerator_FWD_DEFINED__
#define __IXGameSaveProviderEnumerator_FWD_DEFINED__
typedef interface IXGameSaveProviderEnumerator IXGameSaveProviderEnumerator;

#endif 	/* __IXGameSaveProviderEnumerator_FWD_DEFINED__ */


#ifndef __XGameSaveProviderEnumerator_FWD_DEFINED__
#define __XGameSaveProviderEnumerator_FWD_DEFINED__

#ifdef __cplusplus
typedef class XGameSaveProviderEnumerator XGameSaveProviderEnumerator;
#else
typedef struct XGameSaveProviderEnumerator XGameSaveProviderEnumerator;
#endif /* __cplusplus */

#endif 	/* __XGameSaveProviderEnumerator_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "Inspectable.h"
#include "usercontext.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_gamesaveenumerator_0000_0000 */
/* [local] */ 

#define MAX_PROVIDERENUMERATOR_GETITEMS_RANGE 50
typedef struct XGameSaveProviderInfo
    {
    HSTRING Xuid;
    HSTRING Scid;
    HSTRING DisplayName;
    HSTRING Aumid;
    FILETIME LastModified;
    UINT64 TotalBytes;
    boolean InSync;
    boolean IsActive;
    HRESULT LastSyncHr;
    HSTRING Location;
    UINT64 UploadedBytes;
    HSTRING LastOwnerChangeId;
    } 	XGameSaveProviderInfo;

#define XGAMESAVESYNC_STATE_FLAGS_NONE 0x0
#define XGAMESAVESYNC_STATE_FLAGS_NOAUTH 0x1
#define XGAMESAVESYNC_STATE_FLAGS_NO_CONNECTIVITY 0x2
#define XGAMESAVESYNC_STATE_FLAGS_UNSYNCED 0x4
#define XGAMESAVESYNC_STATE_FLAGS_LOST_LOCK 0x8
#define XGAMESAVESYNC_STATE_FLAGS_UNREGISTERED 0x10
#define XGAMESAVESYNC_STATE_FLAGS_NOT_FOUND 0x40
#define XGAMESAVESYNC_STATE_FLAGS_DUPLICATE 0x80
#define XGAMESAVESYNC_STATE_FLAGS_INITIALIZING 0x100
typedef 
enum XGameSaveSyncState
    {
        Unavailable	= 0,
        LocalOffline	= 1,
        LocalChanged	= 2,
        LocalUploading	= 3,
        LocalUploaded	= 4,
        LocalDownloading	= 5,
        LocalDownloaded	= 6,
        RemoteOffline	= 7,
        RemoteUploading	= 8,
        RemoteUploaded	= 9
    } 	XGameSaveSyncState;

typedef struct XGameSaveSyncInfo
    {
    XGameSaveSyncState State;
    UINT32 ProgressPercentage;
    FILETIME LastUpdateTime;
    DWORD Flags;
    } 	XGameSaveSyncInfo;



extern RPC_IF_HANDLE __MIDL_itf_gamesaveenumerator_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveenumerator_0000_0000_v0_0_s_ifspec;

#ifndef __IXGameSaveProviderEnumerator_INTERFACE_DEFINED__
#define __IXGameSaveProviderEnumerator_INTERFACE_DEFINED__

/* interface IXGameSaveProviderEnumerator */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveProviderEnumerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F187A451-AC81-4283-935D-2A2C4797D3D6")
    IXGameSaveProviderEnumerator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetItems( 
            /* [in] */ UINT32 startIndex,
            /* [range][in] */ __RPC__in_range(0,50) UINT32 maxNumberOfItems,
            /* [size_is][out] */ __RPC__out_ecount_full(maxNumberOfItems) XGameSaveProviderInfo items[  ],
            /* [out] */ __RPC__out UINT32 *readCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemCount( 
            /* [out] */ __RPC__out UINT32 *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteLocalData( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING scid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteLocalAndCloudData( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING scid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetXuidFromUserContextToken( 
            /* [in] */ UserContextToken userContext,
            /* [out] */ __RPC__deref_out_opt HSTRING *xuid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QuerySaveSyncStatusFromLocalRecords( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [size_is][in] */ __RPC__in_ecount_full(scidCount) HSTRING *scids,
            /* [in] */ UINT32 scidCount,
            /* [out] */ __RPC__out XGameSaveSyncInfo *info) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddCloudSaveSyncMonitor( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [in] */ __RPC__in HSTRING primaryScid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveCloudSaveSyncMonitor( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPrimaryScid( 
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [out] */ __RPC__deref_out_opt HSTRING *scid) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveProviderEnumeratorVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveProviderEnumerator * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveProviderEnumerator * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, GetItems)
        HRESULT ( STDMETHODCALLTYPE *GetItems )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ UINT32 startIndex,
            /* [range][in] */ __RPC__in_range(0,50) UINT32 maxNumberOfItems,
            /* [size_is][out] */ __RPC__out_ecount_full(maxNumberOfItems) XGameSaveProviderInfo items[  ],
            /* [out] */ __RPC__out UINT32 *readCount);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, GetItemCount)
        HRESULT ( STDMETHODCALLTYPE *GetItemCount )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [out] */ __RPC__out UINT32 *count);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, DeleteLocalData)
        HRESULT ( STDMETHODCALLTYPE *DeleteLocalData )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING scid);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, DeleteLocalAndCloudData)
        HRESULT ( STDMETHODCALLTYPE *DeleteLocalAndCloudData )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING scid);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, GetXuidFromUserContextToken)
        HRESULT ( STDMETHODCALLTYPE *GetXuidFromUserContextToken )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ UserContextToken userContext,
            /* [out] */ __RPC__deref_out_opt HSTRING *xuid);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, QuerySaveSyncStatusFromLocalRecords)
        HRESULT ( STDMETHODCALLTYPE *QuerySaveSyncStatusFromLocalRecords )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [size_is][in] */ __RPC__in_ecount_full(scidCount) HSTRING *scids,
            /* [in] */ UINT32 scidCount,
            /* [out] */ __RPC__out XGameSaveSyncInfo *info);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, AddCloudSaveSyncMonitor)
        HRESULT ( STDMETHODCALLTYPE *AddCloudSaveSyncMonitor )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [in] */ __RPC__in HSTRING primaryScid);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, RemoveCloudSaveSyncMonitor)
        HRESULT ( STDMETHODCALLTYPE *RemoveCloudSaveSyncMonitor )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName);
        
        DECLSPEC_XFGVIRT(IXGameSaveProviderEnumerator, GetPrimaryScid)
        HRESULT ( STDMETHODCALLTYPE *GetPrimaryScid )( 
            __RPC__in IXGameSaveProviderEnumerator * This,
            /* [in] */ __RPC__in HSTRING xuid,
            /* [in] */ __RPC__in HSTRING packageFamilyName,
            /* [out] */ __RPC__deref_out_opt HSTRING *scid);
        
        END_INTERFACE
    } IXGameSaveProviderEnumeratorVtbl;

    interface IXGameSaveProviderEnumerator
    {
        CONST_VTBL struct IXGameSaveProviderEnumeratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveProviderEnumerator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveProviderEnumerator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveProviderEnumerator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveProviderEnumerator_GetItems(This,startIndex,maxNumberOfItems,items,readCount)	\
    ( (This)->lpVtbl -> GetItems(This,startIndex,maxNumberOfItems,items,readCount) ) 

#define IXGameSaveProviderEnumerator_GetItemCount(This,count)	\
    ( (This)->lpVtbl -> GetItemCount(This,count) ) 

#define IXGameSaveProviderEnumerator_DeleteLocalData(This,xuid,scid)	\
    ( (This)->lpVtbl -> DeleteLocalData(This,xuid,scid) ) 

#define IXGameSaveProviderEnumerator_DeleteLocalAndCloudData(This,xuid,scid)	\
    ( (This)->lpVtbl -> DeleteLocalAndCloudData(This,xuid,scid) ) 

#define IXGameSaveProviderEnumerator_GetXuidFromUserContextToken(This,userContext,xuid)	\
    ( (This)->lpVtbl -> GetXuidFromUserContextToken(This,userContext,xuid) ) 

#define IXGameSaveProviderEnumerator_QuerySaveSyncStatusFromLocalRecords(This,xuid,packageFamilyName,scids,scidCount,info)	\
    ( (This)->lpVtbl -> QuerySaveSyncStatusFromLocalRecords(This,xuid,packageFamilyName,scids,scidCount,info) ) 

#define IXGameSaveProviderEnumerator_AddCloudSaveSyncMonitor(This,xuid,packageFamilyName,primaryScid)	\
    ( (This)->lpVtbl -> AddCloudSaveSyncMonitor(This,xuid,packageFamilyName,primaryScid) ) 

#define IXGameSaveProviderEnumerator_RemoveCloudSaveSyncMonitor(This,xuid,packageFamilyName)	\
    ( (This)->lpVtbl -> RemoveCloudSaveSyncMonitor(This,xuid,packageFamilyName) ) 

#define IXGameSaveProviderEnumerator_GetPrimaryScid(This,xuid,packageFamilyName,scid)	\
    ( (This)->lpVtbl -> GetPrimaryScid(This,xuid,packageFamilyName,scid) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveProviderEnumerator_INTERFACE_DEFINED__ */



#ifndef __XGameSaveEnumerator_LIBRARY_DEFINED__
#define __XGameSaveEnumerator_LIBRARY_DEFINED__

/* library XGameSaveEnumerator */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_XGameSaveEnumerator;

EXTERN_C const CLSID CLSID_XGameSaveProviderEnumerator;

#ifdef __cplusplus

class DECLSPEC_UUID("D8C956A7-D22F-461A-857C-89FB1F9C378B")
XGameSaveProviderEnumerator;
#endif
#endif /* __XGameSaveEnumerator_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  HSTRING_UserSize(     __RPC__in unsigned long *, unsigned long            , __RPC__in HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserMarshal(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserUnmarshal(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out HSTRING * ); 
void                      __RPC_USER  HSTRING_UserFree(     __RPC__in unsigned long *, __RPC__in HSTRING * ); 

unsigned long             __RPC_USER  HSTRING_UserSize64(     __RPC__in unsigned long *, unsigned long            , __RPC__in HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserMarshal64(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserUnmarshal64(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out HSTRING * ); 
void                      __RPC_USER  HSTRING_UserFree64(     __RPC__in unsigned long *, __RPC__in HSTRING * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


