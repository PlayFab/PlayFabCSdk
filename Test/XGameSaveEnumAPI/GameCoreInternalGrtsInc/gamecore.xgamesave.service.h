

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

#ifndef __gamesaveservice_h__
#define __gamesaveservice_h__

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

#ifndef __IXGameSaveService_FWD_DEFINED__
#define __IXGameSaveService_FWD_DEFINED__
typedef interface IXGameSaveService IXGameSaveService;

#endif 	/* __IXGameSaveService_FWD_DEFINED__ */


#ifndef __IXGameSaveOperationHandler_FWD_DEFINED__
#define __IXGameSaveOperationHandler_FWD_DEFINED__
typedef interface IXGameSaveOperationHandler IXGameSaveOperationHandler;

#endif 	/* __IXGameSaveOperationHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveOperationHandler_FWD_DEFINED__
#define __AsyncIXGameSaveOperationHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveOperationHandler AsyncIXGameSaveOperationHandler;

#endif 	/* __AsyncIXGameSaveOperationHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveContainerSyncData_FWD_DEFINED__
#define __IXGameSaveContainerSyncData_FWD_DEFINED__
typedef interface IXGameSaveContainerSyncData IXGameSaveContainerSyncData;

#endif 	/* __IXGameSaveContainerSyncData_FWD_DEFINED__ */


#ifndef __IXGameSaveSpaceHandler_FWD_DEFINED__
#define __IXGameSaveSpaceHandler_FWD_DEFINED__
typedef interface IXGameSaveSpaceHandler IXGameSaveSpaceHandler;

#endif 	/* __IXGameSaveSpaceHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveSpaceHandler_FWD_DEFINED__
#define __AsyncIXGameSaveSpaceHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveSpaceHandler AsyncIXGameSaveSpaceHandler;

#endif 	/* __AsyncIXGameSaveSpaceHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveFileSpaceHandler_FWD_DEFINED__
#define __IXGameSaveFileSpaceHandler_FWD_DEFINED__
typedef interface IXGameSaveFileSpaceHandler IXGameSaveFileSpaceHandler;

#endif 	/* __IXGameSaveFileSpaceHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveFileSpaceHandler_FWD_DEFINED__
#define __AsyncIXGameSaveFileSpaceHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveFileSpaceHandler AsyncIXGameSaveFileSpaceHandler;

#endif 	/* __AsyncIXGameSaveFileSpaceHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveInterruptHandler_FWD_DEFINED__
#define __IXGameSaveInterruptHandler_FWD_DEFINED__
typedef interface IXGameSaveInterruptHandler IXGameSaveInterruptHandler;

#endif 	/* __IXGameSaveInterruptHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveReadHandler_FWD_DEFINED__
#define __IXGameSaveReadHandler_FWD_DEFINED__
typedef interface IXGameSaveReadHandler IXGameSaveReadHandler;

#endif 	/* __IXGameSaveReadHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveReadHandler_FWD_DEFINED__
#define __AsyncIXGameSaveReadHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveReadHandler AsyncIXGameSaveReadHandler;

#endif 	/* __AsyncIXGameSaveReadHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveQueryHandler_FWD_DEFINED__
#define __IXGameSaveQueryHandler_FWD_DEFINED__
typedef interface IXGameSaveQueryHandler IXGameSaveQueryHandler;

#endif 	/* __IXGameSaveQueryHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveQueryHandler_FWD_DEFINED__
#define __AsyncIXGameSaveQueryHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveQueryHandler AsyncIXGameSaveQueryHandler;

#endif 	/* __AsyncIXGameSaveQueryHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveNameQuery_FWD_DEFINED__
#define __IXGameSaveNameQuery_FWD_DEFINED__
typedef interface IXGameSaveNameQuery IXGameSaveNameQuery;

#endif 	/* __IXGameSaveNameQuery_FWD_DEFINED__ */


#ifndef __IXGameSaveQuotaHandler_FWD_DEFINED__
#define __IXGameSaveQuotaHandler_FWD_DEFINED__
typedef interface IXGameSaveQuotaHandler IXGameSaveQuotaHandler;

#endif 	/* __IXGameSaveQuotaHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveQuotaHandler_FWD_DEFINED__
#define __AsyncIXGameSaveQuotaHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveQuotaHandler AsyncIXGameSaveQuotaHandler;

#endif 	/* __AsyncIXGameSaveQuotaHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveBlobQueryHandler_FWD_DEFINED__
#define __IXGameSaveBlobQueryHandler_FWD_DEFINED__
typedef interface IXGameSaveBlobQueryHandler IXGameSaveBlobQueryHandler;

#endif 	/* __IXGameSaveBlobQueryHandler_FWD_DEFINED__ */


#ifndef __AsyncIXGameSaveBlobQueryHandler_FWD_DEFINED__
#define __AsyncIXGameSaveBlobQueryHandler_FWD_DEFINED__
typedef interface AsyncIXGameSaveBlobQueryHandler AsyncIXGameSaveBlobQueryHandler;

#endif 	/* __AsyncIXGameSaveBlobQueryHandler_FWD_DEFINED__ */


#ifndef __IXGameSaveBlobNameQuery_FWD_DEFINED__
#define __IXGameSaveBlobNameQuery_FWD_DEFINED__
typedef interface IXGameSaveBlobNameQuery IXGameSaveBlobNameQuery;

#endif 	/* __IXGameSaveBlobNameQuery_FWD_DEFINED__ */


#ifndef __IXGameSaveUIProvider_FWD_DEFINED__
#define __IXGameSaveUIProvider_FWD_DEFINED__
typedef interface IXGameSaveUIProvider IXGameSaveUIProvider;

#endif 	/* __IXGameSaveUIProvider_FWD_DEFINED__ */


#ifndef __IXGameSaveUIProgressCallback_FWD_DEFINED__
#define __IXGameSaveUIProgressCallback_FWD_DEFINED__
typedef interface IXGameSaveUIProgressCallback IXGameSaveUIProgressCallback;

#endif 	/* __IXGameSaveUIProgressCallback_FWD_DEFINED__ */


#ifndef __IXGameSaveUIRetryCallback_FWD_DEFINED__
#define __IXGameSaveUIRetryCallback_FWD_DEFINED__
typedef interface IXGameSaveUIRetryCallback IXGameSaveUIRetryCallback;

#endif 	/* __IXGameSaveUIRetryCallback_FWD_DEFINED__ */


#ifndef __IXGameSaveUILockContentionCallback_FWD_DEFINED__
#define __IXGameSaveUILockContentionCallback_FWD_DEFINED__
typedef interface IXGameSaveUILockContentionCallback IXGameSaveUILockContentionCallback;

#endif 	/* __IXGameSaveUILockContentionCallback_FWD_DEFINED__ */


#ifndef __IXGameSaveUIQuitCallback_FWD_DEFINED__
#define __IXGameSaveUIQuitCallback_FWD_DEFINED__
typedef interface IXGameSaveUIQuitCallback IXGameSaveUIQuitCallback;

#endif 	/* __IXGameSaveUIQuitCallback_FWD_DEFINED__ */


#ifndef __IXGameSaveUIConflictResolutionCallback_FWD_DEFINED__
#define __IXGameSaveUIConflictResolutionCallback_FWD_DEFINED__
typedef interface IXGameSaveUIConflictResolutionCallback IXGameSaveUIConflictResolutionCallback;

#endif 	/* __IXGameSaveUIConflictResolutionCallback_FWD_DEFINED__ */


#ifndef __IXGameSaveUIOutOfLocalStorageCallback_FWD_DEFINED__
#define __IXGameSaveUIOutOfLocalStorageCallback_FWD_DEFINED__
typedef interface IXGameSaveUIOutOfLocalStorageCallback IXGameSaveUIOutOfLocalStorageCallback;

#endif 	/* __IXGameSaveUIOutOfLocalStorageCallback_FWD_DEFINED__ */


#ifndef __XGameSaveService_FWD_DEFINED__
#define __XGameSaveService_FWD_DEFINED__

#ifdef __cplusplus
typedef class XGameSaveService XGameSaveService;
#else
typedef struct XGameSaveService XGameSaveService;
#endif /* __cplusplus */

#endif 	/* __XGameSaveService_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "Inspectable.h"
#include "usercontext.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_gamesaveservice_0000_0000 */
/* [local] */ 

















typedef struct XGameSaveNamedBufferInfo
    {
    HSTRING Name;
    UINT32 Size;
    } 	XGameSaveNamedBufferInfo;

typedef /* [v1_enum] */ 
enum XGAMESAVE_SYNC_STATE
    {
        XGAMESAVE_SYNC_PREPARING	= 0,
        XGAMESAVE_SYNC_COMPLETE	= ( XGAMESAVE_SYNC_PREPARING + 1 ) ,
        XGAMESAVE_SYNC_SYNCING	= ( XGAMESAVE_SYNC_COMPLETE + 1 ) ,
        XGAMESAVE_SYNC_NOT_STARTED	= ( XGAMESAVE_SYNC_SYNCING + 1 ) 
    } 	XGAMESAVE_SYNC_STATE;

typedef /* [v1_enum] */ 
enum XGAMESAVE_SPACE_SYNC_MODE
    {
        XGAMESAVE_SPACE_SYNC_MODE_FULL	= 0,
        XGAMESAVE_SPACE_SYNC_MODE_PARTIAL	= ( XGAMESAVE_SPACE_SYNC_MODE_FULL + 1 ) 
    } 	XGAMESAVE_SPACE_SYNC_MODE;

#define	XGAMESAVE_SYNC_PREPARECONTEXT_FLAGS_NONE	( 0 )

#define	XGAMESAVE_SYNC_PREPARECONTEXT_FLAGS_TRACKCONTAINERSCHANGED	( 0x1 )

#define	XGAMESAVE_SYNC_PREPARECONTEXT_FLAGS_SYNCMODE_CHECK	( 0x2 )

#define	XGAMESAVE_SYNC_PREPARECONTEXT_FLAGS_FILEMODE	( 0x4 )



extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0000_v0_0_s_ifspec;

#ifndef __IXGameSaveService_INTERFACE_DEFINED__
#define __IXGameSaveService_INTERFACE_DEFINED__

/* interface IXGameSaveService */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3AC85287-EEC3-40C4-B86A-853CDCCC0559")
    IXGameSaveService : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PrepareContext( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ XGAMESAVE_SPACE_SYNC_MODE syncMode,
            /* [in] */ UINT32 flags,
            /* [in] */ __RPC__in_opt IXGameSaveInterruptHandler *interruptHandler,
            /* [in] */ __RPC__in_opt IXGameSaveSpaceHandler *spaceHandler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterProvider( 
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in_opt IXGameSaveUIProvider *provider) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Update( 
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ __RPC__in HSTRING containerDisplayName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ UINT32 updateCount,
            /* [size_is][in] */ __RPC__in_ecount_full(updateCount) const XGameSaveNamedBufferInfo updates[  ],
            /* [in] */ UINT32 deleteCount,
            /* [size_is][in] */ __RPC__in_ecount_full(deleteCount) HSTRING deletes[  ],
            /* [in] */ __RPC__in_opt ISequentialStream *stream,
            /* [in] */ __RPC__in_opt IXGameSaveOperationHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) HSTRING blobNames[  ],
            /* [in] */ __RPC__in_opt IXGameSaveReadHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ __RPC__in_opt IXGameSaveOperationHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateQuery( 
            /* [in] */ __RPC__in HSTRING containerNamePrefix,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [retval][out] */ __RPC__deref_out_opt IXGameSaveNameQuery **query) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateBlobQuery( 
            /* [in] */ __RPC__in HSTRING blobNamePrefix,
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [retval][out] */ __RPC__deref_out_opt IXGameSaveBlobNameQuery **query) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQuota( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ __RPC__in_opt IXGameSaveQuotaHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelOperation( 
            /* [in] */ GUID guid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppSuspended( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppResumed( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveServiceVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveService * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveService * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, PrepareContext)
        HRESULT ( STDMETHODCALLTYPE *PrepareContext )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ XGAMESAVE_SPACE_SYNC_MODE syncMode,
            /* [in] */ UINT32 flags,
            /* [in] */ __RPC__in_opt IXGameSaveInterruptHandler *interruptHandler,
            /* [in] */ __RPC__in_opt IXGameSaveSpaceHandler *spaceHandler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, RegisterProvider)
        HRESULT ( STDMETHODCALLTYPE *RegisterProvider )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in_opt IXGameSaveUIProvider *provider);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, Update)
        HRESULT ( STDMETHODCALLTYPE *Update )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ __RPC__in HSTRING containerDisplayName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ UINT32 updateCount,
            /* [size_is][in] */ __RPC__in_ecount_full(updateCount) const XGameSaveNamedBufferInfo updates[  ],
            /* [in] */ UINT32 deleteCount,
            /* [size_is][in] */ __RPC__in_ecount_full(deleteCount) HSTRING deletes[  ],
            /* [in] */ __RPC__in_opt ISequentialStream *stream,
            /* [in] */ __RPC__in_opt IXGameSaveOperationHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, Read)
        HRESULT ( STDMETHODCALLTYPE *Read )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) HSTRING blobNames[  ],
            /* [in] */ __RPC__in_opt IXGameSaveReadHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, Delete)
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ __RPC__in_opt IXGameSaveOperationHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, CreateQuery)
        HRESULT ( STDMETHODCALLTYPE *CreateQuery )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING containerNamePrefix,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [retval][out] */ __RPC__deref_out_opt IXGameSaveNameQuery **query);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, CreateBlobQuery)
        HRESULT ( STDMETHODCALLTYPE *CreateBlobQuery )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ __RPC__in HSTRING blobNamePrefix,
            /* [in] */ __RPC__in HSTRING containerName,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [retval][out] */ __RPC__deref_out_opt IXGameSaveBlobNameQuery **query);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, GetQuota)
        HRESULT ( STDMETHODCALLTYPE *GetQuota )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING scid,
            /* [in] */ __RPC__in_opt IXGameSaveQuotaHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, CancelOperation)
        HRESULT ( STDMETHODCALLTYPE *CancelOperation )( 
            __RPC__in IXGameSaveService * This,
            /* [in] */ GUID guid);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, AppSuspended)
        HRESULT ( STDMETHODCALLTYPE *AppSuspended )( 
            __RPC__in IXGameSaveService * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveService, AppResumed)
        HRESULT ( STDMETHODCALLTYPE *AppResumed )( 
            __RPC__in IXGameSaveService * This);
        
        END_INTERFACE
    } IXGameSaveServiceVtbl;

    interface IXGameSaveService
    {
        CONST_VTBL struct IXGameSaveServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveService_PrepareContext(This,userContext,scid,syncMode,flags,interruptHandler,spaceHandler,operationId)	\
    ( (This)->lpVtbl -> PrepareContext(This,userContext,scid,syncMode,flags,interruptHandler,spaceHandler,operationId) ) 

#define IXGameSaveService_RegisterProvider(This,aumid,provider)	\
    ( (This)->lpVtbl -> RegisterProvider(This,aumid,provider) ) 

#define IXGameSaveService_Update(This,containerName,containerDisplayName,userContext,scid,updateCount,updates,deleteCount,deletes,stream,handler,operationId)	\
    ( (This)->lpVtbl -> Update(This,containerName,containerDisplayName,userContext,scid,updateCount,updates,deleteCount,deletes,stream,handler,operationId) ) 

#define IXGameSaveService_Read(This,containerName,userContext,scid,count,blobNames,handler,operationId)	\
    ( (This)->lpVtbl -> Read(This,containerName,userContext,scid,count,blobNames,handler,operationId) ) 

#define IXGameSaveService_Delete(This,containerName,userContext,scid,handler,operationId)	\
    ( (This)->lpVtbl -> Delete(This,containerName,userContext,scid,handler,operationId) ) 

#define IXGameSaveService_CreateQuery(This,containerNamePrefix,userContext,scid,query)	\
    ( (This)->lpVtbl -> CreateQuery(This,containerNamePrefix,userContext,scid,query) ) 

#define IXGameSaveService_CreateBlobQuery(This,blobNamePrefix,containerName,userContext,scid,query)	\
    ( (This)->lpVtbl -> CreateBlobQuery(This,blobNamePrefix,containerName,userContext,scid,query) ) 

#define IXGameSaveService_GetQuota(This,userContext,scid,handler,operationId)	\
    ( (This)->lpVtbl -> GetQuota(This,userContext,scid,handler,operationId) ) 

#define IXGameSaveService_CancelOperation(This,guid)	\
    ( (This)->lpVtbl -> CancelOperation(This,guid) ) 

#define IXGameSaveService_AppSuspended(This)	\
    ( (This)->lpVtbl -> AppSuspended(This) ) 

#define IXGameSaveService_AppResumed(This)	\
    ( (This)->lpVtbl -> AppResumed(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveService_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveOperationHandler_INTERFACE_DEFINED__
#define __IXGameSaveOperationHandler_INTERFACE_DEFINED__

/* interface IXGameSaveOperationHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveOperationHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CB48C4B7-2ADA-438F-A9CA-E6ACC3838C4B")
    IXGameSaveOperationHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnComplete( 
            /* [in] */ HRESULT result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveOperationHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveOperationHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveOperationHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveOperationHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveOperationHandler, OnComplete)
        HRESULT ( STDMETHODCALLTYPE *OnComplete )( 
            __RPC__in IXGameSaveOperationHandler * This,
            /* [in] */ HRESULT result);
        
        END_INTERFACE
    } IXGameSaveOperationHandlerVtbl;

    interface IXGameSaveOperationHandler
    {
        CONST_VTBL struct IXGameSaveOperationHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveOperationHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveOperationHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveOperationHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveOperationHandler_OnComplete(This,result)	\
    ( (This)->lpVtbl -> OnComplete(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveOperationHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveOperationHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveOperationHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveOperationHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveOperationHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4DAB5B8-A025-4A72-84AC-7FE45C6E5456")
    AsyncIXGameSaveOperationHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnComplete( 
            /* [in] */ HRESULT result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnComplete( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveOperationHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveOperationHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveOperationHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveOperationHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveOperationHandler, Begin_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnComplete )( 
            __RPC__in AsyncIXGameSaveOperationHandler * This,
            /* [in] */ HRESULT result);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveOperationHandler, Finish_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnComplete )( 
            __RPC__in AsyncIXGameSaveOperationHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveOperationHandlerVtbl;

    interface AsyncIXGameSaveOperationHandler
    {
        CONST_VTBL struct AsyncIXGameSaveOperationHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveOperationHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveOperationHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveOperationHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveOperationHandler_Begin_OnComplete(This,result)	\
    ( (This)->lpVtbl -> Begin_OnComplete(This,result) ) 

#define AsyncIXGameSaveOperationHandler_Finish_OnComplete(This)	\
    ( (This)->lpVtbl -> Finish_OnComplete(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveOperationHandler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_gamesaveservice_0000_0002 */
/* [local] */ 

typedef /* [v1_enum] */ 
enum XGAMESAVE_UI_TYPE
    {
        XGAMESAVE_UI_PROGRESS	= 0,
        XGAMESAVE_UI_ERROR	= ( XGAMESAVE_UI_PROGRESS + 1 ) ,
        XGAMESAVE_UI_LOCKCONTENTION	= ( XGAMESAVE_UI_ERROR + 1 ) ,
        XGAMESAVE_UI_CONFLICT	= ( XGAMESAVE_UI_LOCKCONTENTION + 1 ) ,
        XGAMESAVE_UI_OUTOFSTORAGE	= ( XGAMESAVE_UI_CONFLICT + 1 ) ,
        XGAMESAVE_UI_LOCKCONTENTIONWITHCONTEXT	= ( XGAMESAVE_UI_OUTOFSTORAGE + 1 ) ,
        XGAMESAVE_UI_CONFLICTRESOLUTIONWITHCONTEXT	= ( XGAMESAVE_UI_LOCKCONTENTIONWITHCONTEXT + 1 ) ,
        XGAMESAVE_UI_UNKNOWN	= ( XGAMESAVE_UI_CONFLICTRESOLUTIONWITHCONTEXT + 1 ) 
    } 	XGAMESAVE_UI_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0002_v0_0_s_ifspec;

#ifndef __IXGameSaveContainerSyncData_INTERFACE_DEFINED__
#define __IXGameSaveContainerSyncData_INTERFACE_DEFINED__

/* interface IXGameSaveContainerSyncData */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveContainerSyncData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2F3DD6FF-DA47-4AD4-860A-CBA6276C3EF7")
    IXGameSaveContainerSyncData : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [retval][out] */ __RPC__out UINT32 *containerCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContainerNameAt( 
            /* [in] */ UINT32 index,
            /* [retval][out] */ __RPC__deref_out_opt HSTRING *containerName) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveContainerSyncDataVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveContainerSyncData * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveContainerSyncData * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveContainerSyncData * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveContainerSyncData, GetCount)
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            __RPC__in IXGameSaveContainerSyncData * This,
            /* [retval][out] */ __RPC__out UINT32 *containerCount);
        
        DECLSPEC_XFGVIRT(IXGameSaveContainerSyncData, GetContainerNameAt)
        HRESULT ( STDMETHODCALLTYPE *GetContainerNameAt )( 
            __RPC__in IXGameSaveContainerSyncData * This,
            /* [in] */ UINT32 index,
            /* [retval][out] */ __RPC__deref_out_opt HSTRING *containerName);
        
        END_INTERFACE
    } IXGameSaveContainerSyncDataVtbl;

    interface IXGameSaveContainerSyncData
    {
        CONST_VTBL struct IXGameSaveContainerSyncDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveContainerSyncData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveContainerSyncData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveContainerSyncData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveContainerSyncData_GetCount(This,containerCount)	\
    ( (This)->lpVtbl -> GetCount(This,containerCount) ) 

#define IXGameSaveContainerSyncData_GetContainerNameAt(This,index,containerName)	\
    ( (This)->lpVtbl -> GetContainerNameAt(This,index,containerName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveContainerSyncData_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveSpaceHandler_INTERFACE_DEFINED__
#define __IXGameSaveSpaceHandler_INTERFACE_DEFINED__

/* interface IXGameSaveSpaceHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveSpaceHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1A9D8E03-A524-4FC6-A566-2BC802898DFF")
    IXGameSaveSpaceHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnComplete( 
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in_opt IXGameSaveContainerSyncData *containerSyncData) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveSpaceHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveSpaceHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveSpaceHandler, OnComplete)
        HRESULT ( STDMETHODCALLTYPE *OnComplete )( 
            __RPC__in IXGameSaveSpaceHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in_opt IXGameSaveContainerSyncData *containerSyncData);
        
        END_INTERFACE
    } IXGameSaveSpaceHandlerVtbl;

    interface IXGameSaveSpaceHandler
    {
        CONST_VTBL struct IXGameSaveSpaceHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveSpaceHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveSpaceHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveSpaceHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveSpaceHandler_OnComplete(This,result,containerSyncData)	\
    ( (This)->lpVtbl -> OnComplete(This,result,containerSyncData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveSpaceHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveSpaceHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveSpaceHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveSpaceHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveSpaceHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E4C1344D-55A0-453A-957E-83727B36CAC9")
    AsyncIXGameSaveSpaceHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnComplete( 
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in_opt IXGameSaveContainerSyncData *containerSyncData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnComplete( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveSpaceHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveSpaceHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveSpaceHandler, Begin_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnComplete )( 
            __RPC__in AsyncIXGameSaveSpaceHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in_opt IXGameSaveContainerSyncData *containerSyncData);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveSpaceHandler, Finish_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnComplete )( 
            __RPC__in AsyncIXGameSaveSpaceHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveSpaceHandlerVtbl;

    interface AsyncIXGameSaveSpaceHandler
    {
        CONST_VTBL struct AsyncIXGameSaveSpaceHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveSpaceHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveSpaceHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveSpaceHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveSpaceHandler_Begin_OnComplete(This,result,containerSyncData)	\
    ( (This)->lpVtbl -> Begin_OnComplete(This,result,containerSyncData) ) 

#define AsyncIXGameSaveSpaceHandler_Finish_OnComplete(This)	\
    ( (This)->lpVtbl -> Finish_OnComplete(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveSpaceHandler_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveFileSpaceHandler_INTERFACE_DEFINED__
#define __IXGameSaveFileSpaceHandler_INTERFACE_DEFINED__

/* interface IXGameSaveFileSpaceHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveFileSpaceHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1AFDDE03-A524-4FC6-A566-2BC802898DFF")
    IXGameSaveFileSpaceHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnComplete( 
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in HSTRING xgsPath) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveFileSpaceHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveFileSpaceHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveFileSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveFileSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveFileSpaceHandler, OnComplete)
        HRESULT ( STDMETHODCALLTYPE *OnComplete )( 
            __RPC__in IXGameSaveFileSpaceHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in HSTRING xgsPath);
        
        END_INTERFACE
    } IXGameSaveFileSpaceHandlerVtbl;

    interface IXGameSaveFileSpaceHandler
    {
        CONST_VTBL struct IXGameSaveFileSpaceHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveFileSpaceHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveFileSpaceHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveFileSpaceHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveFileSpaceHandler_OnComplete(This,result,xgsPath)	\
    ( (This)->lpVtbl -> OnComplete(This,result,xgsPath) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveFileSpaceHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveFileSpaceHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveFileSpaceHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveFileSpaceHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveFileSpaceHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E4FDD44D-55A0-453A-957E-83727B36CAC9")
    AsyncIXGameSaveFileSpaceHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnComplete( 
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in HSTRING xgsPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnComplete( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveFileSpaceHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveFileSpaceHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveFileSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveFileSpaceHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveFileSpaceHandler, Begin_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnComplete )( 
            __RPC__in AsyncIXGameSaveFileSpaceHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ __RPC__in HSTRING xgsPath);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveFileSpaceHandler, Finish_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnComplete )( 
            __RPC__in AsyncIXGameSaveFileSpaceHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveFileSpaceHandlerVtbl;

    interface AsyncIXGameSaveFileSpaceHandler
    {
        CONST_VTBL struct AsyncIXGameSaveFileSpaceHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveFileSpaceHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveFileSpaceHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveFileSpaceHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveFileSpaceHandler_Begin_OnComplete(This,result,xgsPath)	\
    ( (This)->lpVtbl -> Begin_OnComplete(This,result,xgsPath) ) 

#define AsyncIXGameSaveFileSpaceHandler_Finish_OnComplete(This)	\
    ( (This)->lpVtbl -> Finish_OnComplete(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveFileSpaceHandler_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveInterruptHandler_INTERFACE_DEFINED__
#define __IXGameSaveInterruptHandler_INTERFACE_DEFINED__

/* interface IXGameSaveInterruptHandler */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveInterruptHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A7329866-C529-4493-9FE8-CAAFE0EEDFFD")
    IXGameSaveInterruptHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnUIRequired( 
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_UI_TYPE type) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveInterruptHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveInterruptHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveInterruptHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveInterruptHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveInterruptHandler, OnUIRequired)
        HRESULT ( STDMETHODCALLTYPE *OnUIRequired )( 
            __RPC__in IXGameSaveInterruptHandler * This,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_UI_TYPE type);
        
        END_INTERFACE
    } IXGameSaveInterruptHandlerVtbl;

    interface IXGameSaveInterruptHandler
    {
        CONST_VTBL struct IXGameSaveInterruptHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveInterruptHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveInterruptHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveInterruptHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveInterruptHandler_OnUIRequired(This,aumid,type)	\
    ( (This)->lpVtbl -> OnUIRequired(This,aumid,type) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveInterruptHandler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_gamesaveservice_0000_0006 */
/* [local] */ 

typedef /* [v1_enum] */ 
enum XGAMESAVE_CONTAINER_SYNC_STATE
    {
        XGAMESAVE_CONTAINER_SYNC_STATE_INSYNC	= 0,
        XGAMESAVE_CONTAINER_SYNC_STATE_CLOUDONLY	= ( XGAMESAVE_CONTAINER_SYNC_STATE_INSYNC + 1 ) ,
        XGAMESAVE_CONTAINER_SYNC_STATE_CONFLICT	= ( XGAMESAVE_CONTAINER_SYNC_STATE_CLOUDONLY + 1 ) 
    } 	XGAMESAVE_CONTAINER_SYNC_STATE;

typedef struct XGameSaveContainerQueryInfo
    {
    HSTRING Name;
    UINT64 TotalSize;
    HSTRING DisplayName;
    FILETIME LastModifiedTime;
    XGAMESAVE_CONTAINER_SYNC_STATE SyncState;
    } 	XGameSaveContainerQueryInfo;



extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0006_v0_0_s_ifspec;

#ifndef __IXGameSaveReadHandler_INTERFACE_DEFINED__
#define __IXGameSaveReadHandler_INTERFACE_DEFINED__

/* interface IXGameSaveReadHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveReadHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("115E6AF7-8620-4B0E-A9B1-4CA958B8A24D")
    IXGameSaveReadHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnComplete( 
            /* [in] */ HRESULT result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnBlobReady( 
            /* [in] */ __RPC__in HSTRING blobName,
            /* [in] */ ULONG size,
            /* [in] */ __RPC__in_opt ISequentialStream *stream) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveReadHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveReadHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveReadHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveReadHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveReadHandler, OnComplete)
        HRESULT ( STDMETHODCALLTYPE *OnComplete )( 
            __RPC__in IXGameSaveReadHandler * This,
            /* [in] */ HRESULT result);
        
        DECLSPEC_XFGVIRT(IXGameSaveReadHandler, OnBlobReady)
        HRESULT ( STDMETHODCALLTYPE *OnBlobReady )( 
            __RPC__in IXGameSaveReadHandler * This,
            /* [in] */ __RPC__in HSTRING blobName,
            /* [in] */ ULONG size,
            /* [in] */ __RPC__in_opt ISequentialStream *stream);
        
        END_INTERFACE
    } IXGameSaveReadHandlerVtbl;

    interface IXGameSaveReadHandler
    {
        CONST_VTBL struct IXGameSaveReadHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveReadHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveReadHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveReadHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveReadHandler_OnComplete(This,result)	\
    ( (This)->lpVtbl -> OnComplete(This,result) ) 

#define IXGameSaveReadHandler_OnBlobReady(This,blobName,size,stream)	\
    ( (This)->lpVtbl -> OnBlobReady(This,blobName,size,stream) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveReadHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveReadHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveReadHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveReadHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveReadHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E4D2BF08-1409-4918-9D84-32EE00E9178C")
    AsyncIXGameSaveReadHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnComplete( 
            /* [in] */ HRESULT result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Begin_OnBlobReady( 
            /* [in] */ __RPC__in HSTRING blobName,
            /* [in] */ ULONG size,
            /* [in] */ __RPC__in_opt ISequentialStream *stream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnBlobReady( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveReadHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveReadHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveReadHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveReadHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveReadHandler, Begin_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnComplete )( 
            __RPC__in AsyncIXGameSaveReadHandler * This,
            /* [in] */ HRESULT result);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveReadHandler, Finish_OnComplete)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnComplete )( 
            __RPC__in AsyncIXGameSaveReadHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveReadHandler, Begin_OnBlobReady)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnBlobReady )( 
            __RPC__in AsyncIXGameSaveReadHandler * This,
            /* [in] */ __RPC__in HSTRING blobName,
            /* [in] */ ULONG size,
            /* [in] */ __RPC__in_opt ISequentialStream *stream);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveReadHandler, Finish_OnBlobReady)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnBlobReady )( 
            __RPC__in AsyncIXGameSaveReadHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveReadHandlerVtbl;

    interface AsyncIXGameSaveReadHandler
    {
        CONST_VTBL struct AsyncIXGameSaveReadHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveReadHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveReadHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveReadHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveReadHandler_Begin_OnComplete(This,result)	\
    ( (This)->lpVtbl -> Begin_OnComplete(This,result) ) 

#define AsyncIXGameSaveReadHandler_Finish_OnComplete(This)	\
    ( (This)->lpVtbl -> Finish_OnComplete(This) ) 

#define AsyncIXGameSaveReadHandler_Begin_OnBlobReady(This,blobName,size,stream)	\
    ( (This)->lpVtbl -> Begin_OnBlobReady(This,blobName,size,stream) ) 

#define AsyncIXGameSaveReadHandler_Finish_OnBlobReady(This)	\
    ( (This)->lpVtbl -> Finish_OnBlobReady(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveReadHandler_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveQueryHandler_INTERFACE_DEFINED__
#define __IXGameSaveQueryHandler_INTERFACE_DEFINED__

/* interface IXGameSaveQueryHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveQueryHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7025B35A-849F-49CB-BBFD-EEA00E5C2A01")
    IXGameSaveQueryHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnItemCount( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnItems( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) XGameSaveContainerQueryInfo info[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveQueryHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveQueryHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveQueryHandler, OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *OnItemCount )( 
            __RPC__in IXGameSaveQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count);
        
        DECLSPEC_XFGVIRT(IXGameSaveQueryHandler, OnItems)
        HRESULT ( STDMETHODCALLTYPE *OnItems )( 
            __RPC__in IXGameSaveQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) XGameSaveContainerQueryInfo info[  ]);
        
        END_INTERFACE
    } IXGameSaveQueryHandlerVtbl;

    interface IXGameSaveQueryHandler
    {
        CONST_VTBL struct IXGameSaveQueryHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveQueryHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveQueryHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveQueryHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveQueryHandler_OnItemCount(This,result,count)	\
    ( (This)->lpVtbl -> OnItemCount(This,result,count) ) 

#define IXGameSaveQueryHandler_OnItems(This,result,start,count,info)	\
    ( (This)->lpVtbl -> OnItems(This,result,start,count,info) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveQueryHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveQueryHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveQueryHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveQueryHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveQueryHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("483DCCC8-BEF4-4268-9F88-82D758F22B62")
    AsyncIXGameSaveQueryHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnItemCount( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnItemCount( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Begin_OnItems( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_xcount_full(count) XGameSaveContainerQueryInfo info[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnItems( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveQueryHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQueryHandler, Begin_OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnItemCount )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQueryHandler, Finish_OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnItemCount )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQueryHandler, Begin_OnItems)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnItems )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_xcount_full(count) XGameSaveContainerQueryInfo info[  ]);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQueryHandler, Finish_OnItems)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnItems )( 
            __RPC__in AsyncIXGameSaveQueryHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveQueryHandlerVtbl;

    interface AsyncIXGameSaveQueryHandler
    {
        CONST_VTBL struct AsyncIXGameSaveQueryHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveQueryHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveQueryHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveQueryHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveQueryHandler_Begin_OnItemCount(This,result,count)	\
    ( (This)->lpVtbl -> Begin_OnItemCount(This,result,count) ) 

#define AsyncIXGameSaveQueryHandler_Finish_OnItemCount(This)	\
    ( (This)->lpVtbl -> Finish_OnItemCount(This) ) 

#define AsyncIXGameSaveQueryHandler_Begin_OnItems(This,result,start,count,info)	\
    ( (This)->lpVtbl -> Begin_OnItems(This,result,start,count,info) ) 

#define AsyncIXGameSaveQueryHandler_Finish_OnItems(This)	\
    ( (This)->lpVtbl -> Finish_OnItems(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveQueryHandler_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveNameQuery_INTERFACE_DEFINED__
#define __IXGameSaveNameQuery_INTERFACE_DEFINED__

/* interface IXGameSaveNameQuery */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveNameQuery;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B8040B92-21EA-48C3-882B-45B69FF04AF4")
    IXGameSaveNameQuery : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetItemCount( 
            /* [in] */ __RPC__in_opt IXGameSaveQueryHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItems( 
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [in] */ __RPC__in_opt IXGameSaveQueryHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveNameQueryVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveNameQuery * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveNameQuery * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveNameQuery * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveNameQuery, GetItemCount)
        HRESULT ( STDMETHODCALLTYPE *GetItemCount )( 
            __RPC__in IXGameSaveNameQuery * This,
            /* [in] */ __RPC__in_opt IXGameSaveQueryHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveNameQuery, GetItems)
        HRESULT ( STDMETHODCALLTYPE *GetItems )( 
            __RPC__in IXGameSaveNameQuery * This,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [in] */ __RPC__in_opt IXGameSaveQueryHandler *handler,
            /* [in] */ GUID operationId);
        
        END_INTERFACE
    } IXGameSaveNameQueryVtbl;

    interface IXGameSaveNameQuery
    {
        CONST_VTBL struct IXGameSaveNameQueryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveNameQuery_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveNameQuery_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveNameQuery_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveNameQuery_GetItemCount(This,handler,operationId)	\
    ( (This)->lpVtbl -> GetItemCount(This,handler,operationId) ) 

#define IXGameSaveNameQuery_GetItems(This,start,count,handler,operationId)	\
    ( (This)->lpVtbl -> GetItems(This,start,count,handler,operationId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveNameQuery_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveQuotaHandler_INTERFACE_DEFINED__
#define __IXGameSaveQuotaHandler_INTERFACE_DEFINED__

/* interface IXGameSaveQuotaHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveQuotaHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7685A31F-F733-4246-8547-3DF85BB717A2")
    IXGameSaveQuotaHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnQuota( 
            /* [in] */ HRESULT result,
            /* [in] */ INT64 quotaRemaining) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveQuotaHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveQuotaHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveQuotaHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveQuotaHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveQuotaHandler, OnQuota)
        HRESULT ( STDMETHODCALLTYPE *OnQuota )( 
            __RPC__in IXGameSaveQuotaHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ INT64 quotaRemaining);
        
        END_INTERFACE
    } IXGameSaveQuotaHandlerVtbl;

    interface IXGameSaveQuotaHandler
    {
        CONST_VTBL struct IXGameSaveQuotaHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveQuotaHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveQuotaHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveQuotaHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveQuotaHandler_OnQuota(This,result,quotaRemaining)	\
    ( (This)->lpVtbl -> OnQuota(This,result,quotaRemaining) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveQuotaHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveQuotaHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveQuotaHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveQuotaHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveQuotaHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("41A2EE83-73B2-416D-88F4-4BC1B1FE996D")
    AsyncIXGameSaveQuotaHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnQuota( 
            /* [in] */ HRESULT result,
            /* [in] */ INT64 quotaRemaining) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnQuota( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveQuotaHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveQuotaHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveQuotaHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveQuotaHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQuotaHandler, Begin_OnQuota)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnQuota )( 
            __RPC__in AsyncIXGameSaveQuotaHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ INT64 quotaRemaining);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveQuotaHandler, Finish_OnQuota)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnQuota )( 
            __RPC__in AsyncIXGameSaveQuotaHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveQuotaHandlerVtbl;

    interface AsyncIXGameSaveQuotaHandler
    {
        CONST_VTBL struct AsyncIXGameSaveQuotaHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveQuotaHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveQuotaHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveQuotaHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveQuotaHandler_Begin_OnQuota(This,result,quotaRemaining)	\
    ( (This)->lpVtbl -> Begin_OnQuota(This,result,quotaRemaining) ) 

#define AsyncIXGameSaveQuotaHandler_Finish_OnQuota(This)	\
    ( (This)->lpVtbl -> Finish_OnQuota(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveQuotaHandler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_gamesaveservice_0000_0010 */
/* [local] */ 

typedef struct XGameSaveBlobQueryInfo
    {
    HSTRING Name;
    UINT32 Size;
    } 	XGameSaveBlobQueryInfo;



extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0010_v0_0_s_ifspec;

#ifndef __IXGameSaveBlobQueryHandler_INTERFACE_DEFINED__
#define __IXGameSaveBlobQueryHandler_INTERFACE_DEFINED__

/* interface IXGameSaveBlobQueryHandler */
/* [object][async_uuid][uuid] */ 


EXTERN_C const IID IID_IXGameSaveBlobQueryHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AD6FF479-E54E-4786-AC2A-10D35C5B93A7")
    IXGameSaveBlobQueryHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnItemCount( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnItems( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) XGameSaveBlobQueryInfo info[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveBlobQueryHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveBlobQueryHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveBlobQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveBlobQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveBlobQueryHandler, OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *OnItemCount )( 
            __RPC__in IXGameSaveBlobQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count);
        
        DECLSPEC_XFGVIRT(IXGameSaveBlobQueryHandler, OnItems)
        HRESULT ( STDMETHODCALLTYPE *OnItems )( 
            __RPC__in IXGameSaveBlobQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_ecount_full(count) XGameSaveBlobQueryInfo info[  ]);
        
        END_INTERFACE
    } IXGameSaveBlobQueryHandlerVtbl;

    interface IXGameSaveBlobQueryHandler
    {
        CONST_VTBL struct IXGameSaveBlobQueryHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveBlobQueryHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveBlobQueryHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveBlobQueryHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveBlobQueryHandler_OnItemCount(This,result,count)	\
    ( (This)->lpVtbl -> OnItemCount(This,result,count) ) 

#define IXGameSaveBlobQueryHandler_OnItems(This,result,start,count,info)	\
    ( (This)->lpVtbl -> OnItems(This,result,start,count,info) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveBlobQueryHandler_INTERFACE_DEFINED__ */


#ifndef __AsyncIXGameSaveBlobQueryHandler_INTERFACE_DEFINED__
#define __AsyncIXGameSaveBlobQueryHandler_INTERFACE_DEFINED__

/* interface AsyncIXGameSaveBlobQueryHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_AsyncIXGameSaveBlobQueryHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8F48B00E-45A9-435B-B458-2FFC8FC3AF9E")
    AsyncIXGameSaveBlobQueryHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin_OnItemCount( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnItemCount( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Begin_OnItems( 
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_xcount_full(count) XGameSaveBlobQueryInfo info[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish_OnItems( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct AsyncIXGameSaveBlobQueryHandlerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveBlobQueryHandler, Begin_OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnItemCount )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 count);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveBlobQueryHandler, Finish_OnItemCount)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnItemCount )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveBlobQueryHandler, Begin_OnItems)
        HRESULT ( STDMETHODCALLTYPE *Begin_OnItems )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This,
            /* [in] */ HRESULT result,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [size_is][in] */ __RPC__in_xcount_full(count) XGameSaveBlobQueryInfo info[  ]);
        
        DECLSPEC_XFGVIRT(AsyncIXGameSaveBlobQueryHandler, Finish_OnItems)
        HRESULT ( STDMETHODCALLTYPE *Finish_OnItems )( 
            __RPC__in AsyncIXGameSaveBlobQueryHandler * This);
        
        END_INTERFACE
    } AsyncIXGameSaveBlobQueryHandlerVtbl;

    interface AsyncIXGameSaveBlobQueryHandler
    {
        CONST_VTBL struct AsyncIXGameSaveBlobQueryHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define AsyncIXGameSaveBlobQueryHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define AsyncIXGameSaveBlobQueryHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define AsyncIXGameSaveBlobQueryHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define AsyncIXGameSaveBlobQueryHandler_Begin_OnItemCount(This,result,count)	\
    ( (This)->lpVtbl -> Begin_OnItemCount(This,result,count) ) 

#define AsyncIXGameSaveBlobQueryHandler_Finish_OnItemCount(This)	\
    ( (This)->lpVtbl -> Finish_OnItemCount(This) ) 

#define AsyncIXGameSaveBlobQueryHandler_Begin_OnItems(This,result,start,count,info)	\
    ( (This)->lpVtbl -> Begin_OnItems(This,result,start,count,info) ) 

#define AsyncIXGameSaveBlobQueryHandler_Finish_OnItems(This)	\
    ( (This)->lpVtbl -> Finish_OnItems(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __AsyncIXGameSaveBlobQueryHandler_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveBlobNameQuery_INTERFACE_DEFINED__
#define __IXGameSaveBlobNameQuery_INTERFACE_DEFINED__

/* interface IXGameSaveBlobNameQuery */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveBlobNameQuery;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("142C8431-D185-4F3E-8886-139BFD3430BB")
    IXGameSaveBlobNameQuery : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetItemCount( 
            /* [in] */ __RPC__in_opt IXGameSaveBlobQueryHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItems( 
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [in] */ __RPC__in_opt IXGameSaveBlobQueryHandler *handler,
            /* [in] */ GUID operationId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveBlobNameQueryVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveBlobNameQuery * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveBlobNameQuery * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveBlobNameQuery * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveBlobNameQuery, GetItemCount)
        HRESULT ( STDMETHODCALLTYPE *GetItemCount )( 
            __RPC__in IXGameSaveBlobNameQuery * This,
            /* [in] */ __RPC__in_opt IXGameSaveBlobQueryHandler *handler,
            /* [in] */ GUID operationId);
        
        DECLSPEC_XFGVIRT(IXGameSaveBlobNameQuery, GetItems)
        HRESULT ( STDMETHODCALLTYPE *GetItems )( 
            __RPC__in IXGameSaveBlobNameQuery * This,
            /* [in] */ UINT32 start,
            /* [in] */ UINT32 count,
            /* [in] */ __RPC__in_opt IXGameSaveBlobQueryHandler *handler,
            /* [in] */ GUID operationId);
        
        END_INTERFACE
    } IXGameSaveBlobNameQueryVtbl;

    interface IXGameSaveBlobNameQuery
    {
        CONST_VTBL struct IXGameSaveBlobNameQueryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveBlobNameQuery_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveBlobNameQuery_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveBlobNameQuery_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveBlobNameQuery_GetItemCount(This,handler,operationId)	\
    ( (This)->lpVtbl -> GetItemCount(This,handler,operationId) ) 

#define IXGameSaveBlobNameQuery_GetItems(This,start,count,handler,operationId)	\
    ( (This)->lpVtbl -> GetItems(This,start,count,handler,operationId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveBlobNameQuery_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_gamesaveservice_0000_0012 */
/* [local] */ 

typedef struct XGameSaveUIDeviceInfo
    {
    HSTRING deviceType;
    HSTRING deviceId;
    HSTRING friendlyName;
    FILETIME lockAcquireTime;
    HSTRING thumbnailUri;
    HSTRING shortSaveDescription;
    UINT64 totalBytes;
    UINT64 uploadedBytes;
    } 	XGameSaveUIDeviceInfo;

typedef /* [v1_enum] */ 
enum XGAMESAVE_OUTOFSPACE_OPERATIONTYPE
    {
        XGAMESAVE_OUTOFSPACE_OPERATIONTYPE_SYNC	= 0,
        XGAMESAVE_OUTOFSPACE_OPERATIONTYPE_UPDATE	= ( XGAMESAVE_OUTOFSPACE_OPERATIONTYPE_SYNC + 1 ) 
    } 	XGAMESAVE_OUTOFSPACE_OPERATIONTYPE;

typedef /* [v1_enum] */ 
enum XGAMESAVE_LOCKCONTENTION_STATE
    {
        XGAMESAVE_LOCKCONTENTION_START	= 0,
        XGAMESAVE_LOCKCONTENTION_PROGRESS_UPDATE	= ( XGAMESAVE_LOCKCONTENTION_START + 1 ) ,
        XGAMESAVE_LOCKCONTENTION_LOCK_AVAILABLE	= ( XGAMESAVE_LOCKCONTENTION_PROGRESS_UPDATE + 1 ) 
    } 	XGAMESAVE_LOCKCONTENTION_STATE;



extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gamesaveservice_0000_0012_v0_0_s_ifspec;

#ifndef __IXGameSaveUIProvider_INTERFACE_DEFINED__
#define __IXGameSaveUIProvider_INTERFACE_DEFINED__

/* interface IXGameSaveUIProvider */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B4689D67-E337-42E1-8D0A-03D70D5B3816")
    IXGameSaveUIProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SyncProgress( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_SYNC_STATE syncState,
            /* [in] */ __RPC__in_opt IXGameSaveUIProgressCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SyncProgressContainer( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in HSTRING localContainerDisplayName,
            /* [in] */ __RPC__in HSTRING remoteContainerDisplayName,
            /* [in] */ XGAMESAVE_SYNC_STATE syncState,
            /* [in] */ __RPC__in_opt IXGameSaveUIProgressCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SyncFailed( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in_opt IXGameSaveUIRetryCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockContention( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localTime,
            /* [in] */ FILETIME remoteTime,
            /* [in] */ __RPC__in_opt IXGameSaveUILockContentionCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConflictResolution( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localModifiedTime,
            /* [in] */ FILETIME remoteModifiedTime,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OutOfLocalStorage( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ UINT64 requiredBytes,
            /* [in] */ XGAMESAVE_OUTOFSPACE_OPERATIONTYPE operationType,
            /* [in] */ __RPC__in_opt IXGameSaveUIOutOfLocalStorageCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConflictResolutionWithSize( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localModifiedTime,
            /* [in] */ FILETIME remoteModifiedTime,
            /* [in] */ UINT64 localSize,
            /* [in] */ UINT64 remoteSize,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockContentionWithContext( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_LOCKCONTENTION_STATE lockContentionState,
            /* [in] */ UINT32 uploadProgressPercentage,
            /* [in] */ FILETIME uploadProgressLastUpdateTime,
            /* [in] */ XGameSaveUIDeviceInfo lockHolderDeviceInfo,
            /* [in] */ XGameSaveUIDeviceInfo previousLockHolderDeviceInfo,
            /* [in] */ __RPC__in_opt IXGameSaveUILockContentionCallback *callback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConflictResolutionWithContext( 
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGameSaveUIDeviceInfo localGameSave,
            /* [in] */ XGameSaveUIDeviceInfo remoteGameSave,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIProviderVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIProvider * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIProvider * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, SyncProgress)
        HRESULT ( STDMETHODCALLTYPE *SyncProgress )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_SYNC_STATE syncState,
            /* [in] */ __RPC__in_opt IXGameSaveUIProgressCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, SyncProgressContainer)
        HRESULT ( STDMETHODCALLTYPE *SyncProgressContainer )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in HSTRING localContainerDisplayName,
            /* [in] */ __RPC__in HSTRING remoteContainerDisplayName,
            /* [in] */ XGAMESAVE_SYNC_STATE syncState,
            /* [in] */ __RPC__in_opt IXGameSaveUIProgressCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, SyncFailed)
        HRESULT ( STDMETHODCALLTYPE *SyncFailed )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ __RPC__in_opt IXGameSaveUIRetryCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, LockContention)
        HRESULT ( STDMETHODCALLTYPE *LockContention )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localTime,
            /* [in] */ FILETIME remoteTime,
            /* [in] */ __RPC__in_opt IXGameSaveUILockContentionCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, ConflictResolution)
        HRESULT ( STDMETHODCALLTYPE *ConflictResolution )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localModifiedTime,
            /* [in] */ FILETIME remoteModifiedTime,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, OutOfLocalStorage)
        HRESULT ( STDMETHODCALLTYPE *OutOfLocalStorage )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ UINT64 requiredBytes,
            /* [in] */ XGAMESAVE_OUTOFSPACE_OPERATIONTYPE operationType,
            /* [in] */ __RPC__in_opt IXGameSaveUIOutOfLocalStorageCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, ConflictResolutionWithSize)
        HRESULT ( STDMETHODCALLTYPE *ConflictResolutionWithSize )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ FILETIME localModifiedTime,
            /* [in] */ FILETIME remoteModifiedTime,
            /* [in] */ UINT64 localSize,
            /* [in] */ UINT64 remoteSize,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, LockContentionWithContext)
        HRESULT ( STDMETHODCALLTYPE *LockContentionWithContext )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGAMESAVE_LOCKCONTENTION_STATE lockContentionState,
            /* [in] */ UINT32 uploadProgressPercentage,
            /* [in] */ FILETIME uploadProgressLastUpdateTime,
            /* [in] */ XGameSaveUIDeviceInfo lockHolderDeviceInfo,
            /* [in] */ XGameSaveUIDeviceInfo previousLockHolderDeviceInfo,
            /* [in] */ __RPC__in_opt IXGameSaveUILockContentionCallback *callback);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProvider, ConflictResolutionWithContext)
        HRESULT ( STDMETHODCALLTYPE *ConflictResolutionWithContext )( 
            __RPC__in IXGameSaveUIProvider * This,
            /* [in] */ UserContextToken userContext,
            /* [in] */ __RPC__in HSTRING aumid,
            /* [in] */ XGameSaveUIDeviceInfo localGameSave,
            /* [in] */ XGameSaveUIDeviceInfo remoteGameSave,
            /* [in] */ __RPC__in_opt IXGameSaveUIConflictResolutionCallback *callback);
        
        END_INTERFACE
    } IXGameSaveUIProviderVtbl;

    interface IXGameSaveUIProvider
    {
        CONST_VTBL struct IXGameSaveUIProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIProvider_SyncProgress(This,userContext,aumid,syncState,callback)	\
    ( (This)->lpVtbl -> SyncProgress(This,userContext,aumid,syncState,callback) ) 

#define IXGameSaveUIProvider_SyncProgressContainer(This,userContext,aumid,localContainerDisplayName,remoteContainerDisplayName,syncState,callback)	\
    ( (This)->lpVtbl -> SyncProgressContainer(This,userContext,aumid,localContainerDisplayName,remoteContainerDisplayName,syncState,callback) ) 

#define IXGameSaveUIProvider_SyncFailed(This,userContext,aumid,callback)	\
    ( (This)->lpVtbl -> SyncFailed(This,userContext,aumid,callback) ) 

#define IXGameSaveUIProvider_LockContention(This,userContext,aumid,localTime,remoteTime,callback)	\
    ( (This)->lpVtbl -> LockContention(This,userContext,aumid,localTime,remoteTime,callback) ) 

#define IXGameSaveUIProvider_ConflictResolution(This,userContext,aumid,localModifiedTime,remoteModifiedTime,callback)	\
    ( (This)->lpVtbl -> ConflictResolution(This,userContext,aumid,localModifiedTime,remoteModifiedTime,callback) ) 

#define IXGameSaveUIProvider_OutOfLocalStorage(This,userContext,aumid,requiredBytes,operationType,callback)	\
    ( (This)->lpVtbl -> OutOfLocalStorage(This,userContext,aumid,requiredBytes,operationType,callback) ) 

#define IXGameSaveUIProvider_ConflictResolutionWithSize(This,userContext,aumid,localModifiedTime,remoteModifiedTime,localSize,remoteSize,callback)	\
    ( (This)->lpVtbl -> ConflictResolutionWithSize(This,userContext,aumid,localModifiedTime,remoteModifiedTime,localSize,remoteSize,callback) ) 

#define IXGameSaveUIProvider_LockContentionWithContext(This,userContext,aumid,lockContentionState,uploadProgressPercentage,uploadProgressLastUpdateTime,lockHolderDeviceInfo,previousLockHolderDeviceInfo,callback)	\
    ( (This)->lpVtbl -> LockContentionWithContext(This,userContext,aumid,lockContentionState,uploadProgressPercentage,uploadProgressLastUpdateTime,lockHolderDeviceInfo,previousLockHolderDeviceInfo,callback) ) 

#define IXGameSaveUIProvider_ConflictResolutionWithContext(This,userContext,aumid,localGameSave,remoteGameSave,callback)	\
    ( (This)->lpVtbl -> ConflictResolutionWithContext(This,userContext,aumid,localGameSave,remoteGameSave,callback) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIProvider_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUIProgressCallback_INTERFACE_DEFINED__
#define __IXGameSaveUIProgressCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUIProgressCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIProgressCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D48B9253-BA66-46A7-AC85-8DA49F3A7EFD")
    IXGameSaveUIProgressCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgress( 
            /* [out] */ __RPC__out UINT64 *current,
            /* [out] */ __RPC__out UINT64 *total) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIProgressCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIProgressCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIProgressCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIProgressCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProgressCallback, Cancel)
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            __RPC__in IXGameSaveUIProgressCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIProgressCallback, GetProgress)
        HRESULT ( STDMETHODCALLTYPE *GetProgress )( 
            __RPC__in IXGameSaveUIProgressCallback * This,
            /* [out] */ __RPC__out UINT64 *current,
            /* [out] */ __RPC__out UINT64 *total);
        
        END_INTERFACE
    } IXGameSaveUIProgressCallbackVtbl;

    interface IXGameSaveUIProgressCallback
    {
        CONST_VTBL struct IXGameSaveUIProgressCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIProgressCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIProgressCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIProgressCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIProgressCallback_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#define IXGameSaveUIProgressCallback_GetProgress(This,current,total)	\
    ( (This)->lpVtbl -> GetProgress(This,current,total) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIProgressCallback_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUIRetryCallback_INTERFACE_DEFINED__
#define __IXGameSaveUIRetryCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUIRetryCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIRetryCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ECE018C1-E6CD-4B6B-9C1C-16CAB7D0EA6E")
    IXGameSaveUIRetryCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Retry( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetError( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIRetryCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIRetryCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIRetryCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIRetryCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIRetryCallback, Retry)
        HRESULT ( STDMETHODCALLTYPE *Retry )( 
            __RPC__in IXGameSaveUIRetryCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIRetryCallback, Cancel)
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            __RPC__in IXGameSaveUIRetryCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIRetryCallback, GetError)
        HRESULT ( STDMETHODCALLTYPE *GetError )( 
            __RPC__in IXGameSaveUIRetryCallback * This);
        
        END_INTERFACE
    } IXGameSaveUIRetryCallbackVtbl;

    interface IXGameSaveUIRetryCallback
    {
        CONST_VTBL struct IXGameSaveUIRetryCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIRetryCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIRetryCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIRetryCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIRetryCallback_Retry(This)	\
    ( (This)->lpVtbl -> Retry(This) ) 

#define IXGameSaveUIRetryCallback_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#define IXGameSaveUIRetryCallback_GetError(This)	\
    ( (This)->lpVtbl -> GetError(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIRetryCallback_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUILockContentionCallback_INTERFACE_DEFINED__
#define __IXGameSaveUILockContentionCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUILockContentionCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUILockContentionCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C67882B9-127A-4D99-A424-EAE92313BBD5")
    IXGameSaveUILockContentionCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BreakLock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoNotBreakLock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Retry( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUILockContentionCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUILockContentionCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUILockContentionCallback, BreakLock)
        HRESULT ( STDMETHODCALLTYPE *BreakLock )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUILockContentionCallback, DoNotBreakLock)
        HRESULT ( STDMETHODCALLTYPE *DoNotBreakLock )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUILockContentionCallback, Retry)
        HRESULT ( STDMETHODCALLTYPE *Retry )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUILockContentionCallback, Cancel)
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            __RPC__in IXGameSaveUILockContentionCallback * This);
        
        END_INTERFACE
    } IXGameSaveUILockContentionCallbackVtbl;

    interface IXGameSaveUILockContentionCallback
    {
        CONST_VTBL struct IXGameSaveUILockContentionCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUILockContentionCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUILockContentionCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUILockContentionCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUILockContentionCallback_BreakLock(This)	\
    ( (This)->lpVtbl -> BreakLock(This) ) 

#define IXGameSaveUILockContentionCallback_DoNotBreakLock(This)	\
    ( (This)->lpVtbl -> DoNotBreakLock(This) ) 

#define IXGameSaveUILockContentionCallback_Retry(This)	\
    ( (This)->lpVtbl -> Retry(This) ) 

#define IXGameSaveUILockContentionCallback_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUILockContentionCallback_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUIQuitCallback_INTERFACE_DEFINED__
#define __IXGameSaveUIQuitCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUIQuitCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIQuitCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BAA04F60-B117-43C0-8FBA-612E12BA57C4")
    IXGameSaveUIQuitCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Quit( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIQuitCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIQuitCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIQuitCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIQuitCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIQuitCallback, Quit)
        HRESULT ( STDMETHODCALLTYPE *Quit )( 
            __RPC__in IXGameSaveUIQuitCallback * This);
        
        END_INTERFACE
    } IXGameSaveUIQuitCallbackVtbl;

    interface IXGameSaveUIQuitCallback
    {
        CONST_VTBL struct IXGameSaveUIQuitCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIQuitCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIQuitCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIQuitCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIQuitCallback_Quit(This)	\
    ( (This)->lpVtbl -> Quit(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIQuitCallback_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUIConflictResolutionCallback_INTERFACE_DEFINED__
#define __IXGameSaveUIConflictResolutionCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUIConflictResolutionCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIConflictResolutionCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8856634F-2E22-481D-B9CA-EE876CBB5D26")
    IXGameSaveUIConflictResolutionCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TakeLocal( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TakeRemote( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIConflictResolutionCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIConflictResolutionCallback, TakeLocal)
        HRESULT ( STDMETHODCALLTYPE *TakeLocal )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIConflictResolutionCallback, TakeRemote)
        HRESULT ( STDMETHODCALLTYPE *TakeRemote )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIConflictResolutionCallback, Cancel)
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            __RPC__in IXGameSaveUIConflictResolutionCallback * This);
        
        END_INTERFACE
    } IXGameSaveUIConflictResolutionCallbackVtbl;

    interface IXGameSaveUIConflictResolutionCallback
    {
        CONST_VTBL struct IXGameSaveUIConflictResolutionCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIConflictResolutionCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIConflictResolutionCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIConflictResolutionCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIConflictResolutionCallback_TakeLocal(This)	\
    ( (This)->lpVtbl -> TakeLocal(This) ) 

#define IXGameSaveUIConflictResolutionCallback_TakeRemote(This)	\
    ( (This)->lpVtbl -> TakeRemote(This) ) 

#define IXGameSaveUIConflictResolutionCallback_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIConflictResolutionCallback_INTERFACE_DEFINED__ */


#ifndef __IXGameSaveUIOutOfLocalStorageCallback_INTERFACE_DEFINED__
#define __IXGameSaveUIOutOfLocalStorageCallback_INTERFACE_DEFINED__

/* interface IXGameSaveUIOutOfLocalStorageCallback */
/* [object][uuid] */ 


EXTERN_C const IID IID_IXGameSaveUIOutOfLocalStorageCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E9308F43-3F62-44BF-A5FF-456C8254BF3F")
    IXGameSaveUIOutOfLocalStorageCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SpaceCleared( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IXGameSaveUIOutOfLocalStorageCallbackVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __RPC__in IXGameSaveUIOutOfLocalStorageCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __RPC__in IXGameSaveUIOutOfLocalStorageCallback * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __RPC__in IXGameSaveUIOutOfLocalStorageCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIOutOfLocalStorageCallback, SpaceCleared)
        HRESULT ( STDMETHODCALLTYPE *SpaceCleared )( 
            __RPC__in IXGameSaveUIOutOfLocalStorageCallback * This);
        
        DECLSPEC_XFGVIRT(IXGameSaveUIOutOfLocalStorageCallback, Cancel)
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            __RPC__in IXGameSaveUIOutOfLocalStorageCallback * This);
        
        END_INTERFACE
    } IXGameSaveUIOutOfLocalStorageCallbackVtbl;

    interface IXGameSaveUIOutOfLocalStorageCallback
    {
        CONST_VTBL struct IXGameSaveUIOutOfLocalStorageCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXGameSaveUIOutOfLocalStorageCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXGameSaveUIOutOfLocalStorageCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXGameSaveUIOutOfLocalStorageCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXGameSaveUIOutOfLocalStorageCallback_SpaceCleared(This)	\
    ( (This)->lpVtbl -> SpaceCleared(This) ) 

#define IXGameSaveUIOutOfLocalStorageCallback_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IXGameSaveUIOutOfLocalStorageCallback_INTERFACE_DEFINED__ */



#ifndef __XGameSaveService_LIBRARY_DEFINED__
#define __XGameSaveService_LIBRARY_DEFINED__

/* library XGameSaveService */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_XGameSaveService;

EXTERN_C const CLSID CLSID_XGameSaveService;

#ifdef __cplusplus

class DECLSPEC_UUID("710318A4-861A-4599-9DA2-50C84EE59ED8")
XGameSaveService;
#endif
#endif /* __XGameSaveService_LIBRARY_DEFINED__ */

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


