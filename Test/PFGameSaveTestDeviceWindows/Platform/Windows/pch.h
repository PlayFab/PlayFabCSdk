// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include <winsdkver.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#include <sdkddkver.h>

#define RETURN_HR(hr)           return(hr)
#define RETURN_IF_FAILED(hr)    do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { RETURN_HR(__hrRet); }} while (0, 0)

// Use the C++ standard templated min/max
#define NOMINMAX

// Comment out NOGDI to enable GDI functions needed for window text
// #define NODRAWTEXT
// #define NOGDI
// #define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
// Add specific GDI headers
#include <wingdi.h>

#include <wrl/client.h>
#include <wrl/event.h>

#include <grdk.h>

#if _GRDK_VER < 0x55F00C58 /* GDK Edition 220300 */
#error This project requires the March 2022 GDK or later
#endif

#ifdef USING_DIRECTX_HEADERS
#include <directx/dxgiformat.h>
#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxguids/dxguids.h>
#else
#include <d3d12.h>
#endif

#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <exception>
#include <iterator>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <tuple>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <iomanip>
#include <shellapi.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <XGameRuntime.h>
#include <xsapi-c/services_c.h>

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) noexcept : result(hr) {}

        const char* what() const noexcept override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw com_exception(hr);
        }
    }
}


#include <string>
#include <vector>
#include <playfab/services/PFServices.h>
#include <playfab/services/PFAccountManagement.h>
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFLocalUser.h>


extern void ExitGame() noexcept;

#include "Common/DeviceLogging.h"

// SigninXbox.h
HRESULT Sample_Xbox_AttemptSignIn();

#if STEAMWORKS_AVAILABLE
HRESULT Sample_Steam_TryInit();
HRESULT Sample_Steam_AttemptLink(PFServiceConfigHandle serviceConfigHandle, PFLocalUserHandle* localUserHandle);
HRESULT Sample_Steam_AttemptSignIn(PFServiceConfigHandle serviceConfigHandle, PFLocalUserHandle localUserHandle);
#endif

inline std::string GetStringFromU8String(const std::string& u8str)
{
    // std::string in the PlayFab codebase is assumed to be UTF-8 encoded, so we can interop directly with std::u8string required in newer clang
    return std::string(u8str.begin(), u8str.end());
}


