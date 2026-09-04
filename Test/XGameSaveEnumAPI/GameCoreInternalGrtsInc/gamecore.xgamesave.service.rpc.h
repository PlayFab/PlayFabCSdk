

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


#ifndef __xgamesave2Eservice2Erpc_h__
#define __xgamesave2Eservice2Erpc_h__

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

/* header files for imported files */
#include "wtypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IXGameSaveRpc_INTERFACE_DEFINED__
#define __IXGameSaveRpc_INTERFACE_DEFINED__

/* interface IXGameSaveRpc */
/* [unique][version][uuid] */ 

/* client prototype */
HRESULT cliScheduleTaskOperation( 
    /* [in] */ handle_t hRpc,
    /* [string][in] */ __RPC__in_string LPCWSTR operationName,
    /* [in] */ DWORD flags);
/* server prototype */
HRESULT svcScheduleTaskOperation( 
    /* [in] */ handle_t hRpc,
    /* [string][in] */ __RPC__in_string LPCWSTR operationName,
    /* [in] */ DWORD flags);



extern RPC_IF_HANDLE cliIXGameSaveRpc_v1_0_c_ifspec;
extern RPC_IF_HANDLE IXGameSaveRpc_v1_0_c_ifspec;
extern RPC_IF_HANDLE svcIXGameSaveRpc_v1_0_s_ifspec;
#endif /* __IXGameSaveRpc_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


