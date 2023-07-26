// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/httpClient/PFHCPal.h>

#if !HC_PLATFORM_IS_MICROSOFT
#ifndef _Null_terminated_
#define _Null_terminated_
#endif
#endif // HC_PLATFORM_IS_MICROSOFT

#if HC_PLATFORM_IS_MICROSOFT || HC_PLATFORM_IS_PLAYSTATION
#define PF_API_ATTRIBUTES __declspec(dllexport)
#else
#define PF_API_ATTRIBUTES
#endif

#ifndef STDAPICALLTYPE
#define STDAPICALLTYPE
#endif

#define PF_API PF_API_ATTRIBUTES HRESULT STDAPICALLTYPE
#define PF_API_(type) PF_API_ATTRIBUTES type STDAPICALLTYPE

#if HC_PLATFORM_IS_MICROSOFT
    #if _MSC_VER >= 1900
        #define PF_DEPRECATED __declspec(deprecated)
    #else
        #define PF_DEPRECATED
    #endif
#else
    #define PF_DEPRECATED
#endif
#define PF_API_DEPRECATED           PF_API_ATTRIBUTES PF_DEPRECATED HRESULT STDAPICALLTYPE
#define PF_API_DEPRECATED_(type)    PF_API_ATTRIBUTES PF_DEPRECATED type STDAPICALLTYPE

#ifndef _Maybenull_
#define _Maybenull_
#endif
