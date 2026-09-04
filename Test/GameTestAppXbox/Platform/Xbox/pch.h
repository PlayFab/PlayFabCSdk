// Copyright (C) Microsoft Corporation. All rights reserved.
// pch.h - Precompiled header for GameTestAppXbox
//
// Based on PlayFabGameSaveSample-XboxConsole/pch.h with PlayFab SDK additions.

#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <wrl/client.h>
#include <wrl/event.h>

#include <grdk.h>

#if _GRDK_VER < 0x65F41800 /* GDK Edition 251000 */
#error This project requires the October 2025 GDK or later
#endif

#ifdef _GAMING_XBOX_SCARLETT
#include <d3d12_xs.h>
#include <d3dx12_xs.h>
#elif defined(_GAMING_XBOX)
#include <d3d12_x.h>
#include <d3dx12_x.h>
#else
#include <d3d12.h>
#include <dxgi1_6.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "d3dx12.h"
#endif

#define _XM_NO_XMVECTOR_OVERLOADS_

#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <vector>

#ifdef _GAMING_XBOX
#include <pix3.h>
#else
#include <pix.h>
#endif
#include "xal\xal.h"
#include "xsapi-c\services_c.h"

#include <XUser.h>
#include <XTaskQueue.h>
#include <XGame.h>
#include <XGameRuntime.h>
#include <XGameRuntimeInit.h>
#include <XSystem.h>

#ifdef _GAMING_XBOX
#include <appnotify.h>
#endif

#include "DescriptorHeap.h"
#include "ResourceUploadBatch.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "DirectXHelpers.h"
#include "GamePad.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "RenderTargetState.h"

#include "TextConsole.h"

namespace DX
{
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

    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
#ifdef _DEBUG
            char str[64] = {};
            sprintf_s(str, "**ERROR** Fatal Error with HRESULT of %08X\n", static_cast<unsigned int>(hr));
            OutputDebugStringA(str);
            __debugbreak();
#endif
            throw com_exception(hr);
        }
    }
}

#ifndef RETURN_HR
#define RETURN_HR(hr)                                           return(hr)
#endif
#ifndef RETURN_IF_FAILED
#define RETURN_IF_FAILED(hr)                                    do { HRESULT __hrRet = hr; if (FAILED(__hrRet)) { RETURN_HR(__hrRet); }} while (0, 0)
#endif
#ifndef RETURN_HR_IF
#define RETURN_HR_IF(hr, condition)                              do { if (condition) { RETURN_HR(hr); }} while (0, 0)
#endif

// Note: The Xbox console sample has a #pragma warning(default : ...) here that promotes
// warnings like C4365. Omitted because the shared test device code doesn't compile cleanly with it.

// --- PlayFab SDK ---
#include <playfab/services/PFServices.h>
#include <playfab/services/PFAccountManagement.h>
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFLocalUser.h>

extern void ExitGame() noexcept;

#include "DeviceLogging.h"

HRESULT Sample_Xbox_AttemptSignIn();

#if STEAMWORKS_AVAILABLE
HRESULT Sample_Steam_TryInit();
HRESULT Sample_Steam_AttemptLink(PFServiceConfigHandle serviceConfigHandle, PFLocalUserHandle* localUserHandle);
HRESULT Sample_Steam_AttemptSignIn(PFServiceConfigHandle serviceConfigHandle, PFLocalUserHandle localUserHandle);
#endif

inline std::string GetStringFromU8String(const std::string& u8str)
{
    return std::string(u8str.begin(), u8str.end());
}
