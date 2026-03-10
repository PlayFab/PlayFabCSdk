#include "stdafx.h"
#include "Platform/Platform.h"
#include "Platform/Generic/LocalStorage_Generic.h"
#include <Windows.h>
#include "Common/PFCoreGlobalState.h"
#ifdef STEAMWORKS_AVAILABLE
#include "LocalUser_Steam.h"
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <XSystem.h>
#endif

namespace PlayFab
{

// Registry paths and value names
constexpr wchar_t kGrtsSettingsKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OneSettings\\xbox\\GamingRuntimeServices\\Settings\\";
constexpr wchar_t kGamingServicesKey[] = L"SOFTWARE\\Microsoft\\GamingServices";
constexpr wchar_t kForceUseLocalServicesValue[] = L"ForceUseLocalServices";

// Returns true when running on a GDK Xbox console (not Unknown and not Pc)
bool IsRunningOnXboxConsoleGdk() noexcept
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    XSystemDeviceType deviceType = XSystemGetDeviceType();
    return (deviceType != XSystemDeviceType::Unknown && deviceType != XSystemDeviceType::Pc);
#else
    return false;
#endif
}

// Returns true if the out-of-proc GRTS registry key exists
bool IsOutOfProcGRTSAvailable() noexcept
{
    HKEY hKey = nullptr;
    LONG lResult = ERROR_SUCCESS;

    try
    {
        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGrtsSettingsKey, 0, KEY_READ, &hKey);
    }
    catch (const std::exception& e)
    {
        TRACE_ERROR("GRTS registry key failed to open with EXCEPTION: %s", e.what());
        return false;
    }

    if (lResult != ERROR_SUCCESS)
    {
        // Expected ERROR_FILE_NOT_FOUND if registry key not found (e.g., when there is no registry on Steam Deck)
        if (lResult != ERROR_FILE_NOT_FOUND)
        {
            TRACE_ERROR("GRTS registry key failed to open with error: %d", lResult);
        }
        else
        {
            TRACE_VERBOSE("IsOutOfProcGRTSAvailable: registry key not found (expected on some devices) -> false");
        }
        return false;
    }

    RegCloseKey(hKey);
    TRACE_VERBOSE("IsOutOfProcGRTSAvailable: true");
    return true;
}

// Returns true if ForceUseLocalServices is present and equals 1
bool IsForceUseLocalServicesEnabled() noexcept
{
    HKEY hKey = nullptr;
    LONG lResult = ERROR_SUCCESS;

    try
    {
        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGamingServicesKey, 0, KEY_READ, &hKey);
        if (lResult == ERROR_SUCCESS)
        {
            DWORD value = 0;
            DWORD dataSize = sizeof(DWORD);
            lResult = RegQueryValueEx(hKey, kForceUseLocalServicesValue, nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &dataSize);
            if (lResult == ERROR_SUCCESS && value == 1)
            {
                RegCloseKey(hKey);
                TRACE_INFORMATION("IsForceUseLocalServicesEnabled: true");
                return true;
            }
        }
    }
    catch (const std::exception& e)
    {
        TRACE_ERROR("ForceUseLocalServices registry key failed to open with EXCEPTION: %s", e.what());
    }

    if (hKey != nullptr)
    {
        RegCloseKey(hKey);
    }
    TRACE_VERBOSE("IsForceUseLocalServicesEnabled: false");
    return false;
}

HRESULT PlatformInitialize() noexcept
{
    // Setup generic LocalStorage handlers if custom ones weren't set
    PFLocalStorageHooks& storageHooks = GetLocalStorageHandlers();
    if (!storageHooks.read)
    {
        storageHooks.read = Detail::GenericLocalStorageReadAsync;
        storageHooks.write = Detail::GenericLocalStorageWriteAsync;
        storageHooks.clear = Detail::GenericLocalStorageClearAsync;
    }
    return S_OK;
}

HRESULT PlatformGetPlatformType(PlatformInfo& platformInfo, PFPlatformType& platformType) noexcept
{
    TRACE_INFORMATION("Platform_Win32::PlatformGetPlatformType");
    platformType = PFPlatformType::Windows;
    platformInfo = PlatformInfo::None;

    // If on Xbox console via GDK, GRTS is available
    if (IsRunningOnXboxConsoleGdk())
    {
        platformType = PFPlatformType::Xbox;
        platformInfo = PlatformInfo::GRTSAvailable;
        TRACE_INFORMATION("PlatformGetPlatformType: GDK Xbox console detected -> platformType=Xbox, GRTSAvailable");
    }
    else
    {
        // If out-of-proc GRTS is not available, keep defaults
        if (!IsOutOfProcGRTSAvailable())
        {
            TRACE_INFORMATION("PlatformGetPlatformType: Out-of-proc GRTS not available -> using defaults (Windows, None)");
        }
        else if (IsForceUseLocalServicesEnabled())
        {
            // If forced to use local services, keep defaults
            TRACE_INFORMATION("PlatformGetPlatformType: ForceUseLocalServices enabled -> using defaults (Windows, None)");
        }
        else
        {
            // Otherwise, GRTS is available
            platformInfo = PlatformInfo::GRTSAvailable;
            TRACE_INFORMATION("PlatformGetPlatformType: Out-of-proc GRTS available -> platformInfo=GRTSAvailable");
        }
    }

#ifdef STEAMWORKS_AVAILABLE
    if( IsRunningOnSteam() )
    {        
        if (platformInfo == PlatformInfo::GRTSAvailable)
        {
            TRACE_INFORMATION("PlatformGetPlatformType: Running on Steam - setting platformType=SteamPc");
            platformType = PFPlatformType::SteamPc;
        }
        else
        {
            TRACE_INFORMATION("PlatformGetPlatformType: Running on Steam w/o GRTS - setting platformType=SteamDeck");
            platformType = PFPlatformType::SteamDeck;
        }
    }
#endif

    return S_OK;
}

}
