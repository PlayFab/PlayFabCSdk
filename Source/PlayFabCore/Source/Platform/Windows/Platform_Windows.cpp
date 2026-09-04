#include "stdafx.h"
#include "Platform/Platform.h"
#include "Platform/Generic/LocalStorage_Generic.h"
#include <Windows.h>
#include <appmodel.h>
#include "Common/PFCoreGlobalState.h"
#include "LocalUser_Steam.h"
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <XSystem.h>
#endif

namespace PlayFab
{

// Registry paths and value names
constexpr wchar_t kGrtsSettingsKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OneSettings\\xbox\\GamingRuntimeServices\\Settings\\";
constexpr wchar_t kGamingServicesKey[] = L"SOFTWARE\\Microsoft\\GamingServices";
constexpr wchar_t kForceUseLocalServicesValue[] = L"ForceUseLocalServices";

// Dynamic loading for GetPackagesByPackageFamily — may not be available on Proton/Wine.
using PFN_GetPackagesByPackageFamily = LONG (WINAPI*)(PCWSTR, UINT32*, PWSTR*, UINT32*, WCHAR*);

static FARPROC LoadGetPackagesByPackageFamilyFn() noexcept
{
    static HMODULE s_kernel32Dll = []() noexcept -> HMODULE
    {
        return LoadLibraryExW(L"kernel32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }();

    if (s_kernel32Dll == nullptr)
    {
        return nullptr;
    }

    return GetProcAddress(s_kernel32Dll, "GetPackagesByPackageFamily");
}

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

// RAII wrapper for HKEY to ensure RegCloseKey is always called
struct RegKeyDeleter
{
    void operator()(HKEY key) const noexcept { if (key) RegCloseKey(key); }
};
using UniqueRegKey = std::unique_ptr<std::remove_pointer_t<HKEY>, RegKeyDeleter>;

// Returns true if the out-of-proc GRTS registry key exists
bool IsOutOfProcGRTSAvailable() noexcept
{
    HKEY hKeyRaw = nullptr;
    LONG lResult = ERROR_SUCCESS;

    try
    {
        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGrtsSettingsKey, 0, KEY_READ, &hKeyRaw);
    }
    catch (const std::exception& e)
    {
        TRACE_ERROR("GRTS registry key failed to open with EXCEPTION: %s", e.what());
        return false;
    }

    UniqueRegKey hKey{ hKeyRaw };

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

    TRACE_VERBOSE("IsOutOfProcGRTSAvailable: true");
    return true;
}

// Returns true if ForceUseLocalServices is present and equals 1
bool IsForceUseLocalServicesEnabled() noexcept
{
    HKEY hKeyRaw = nullptr;
    LONG lResult = ERROR_SUCCESS;

    try
    {
        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGamingServicesKey, 0, KEY_READ, &hKeyRaw);
        UniqueRegKey hKey{ hKeyRaw };

        if (lResult == ERROR_SUCCESS)
        {
            DWORD value = 0;
            DWORD dataSize = sizeof(DWORD);
            lResult = RegQueryValueEx(hKey.get(), kForceUseLocalServicesValue, nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &dataSize);
            if (lResult == ERROR_SUCCESS && value == 1)
            {
                TRACE_INFORMATION("IsForceUseLocalServicesEnabled: true");
                return true;
            }
        }
    }
    catch (const std::exception& e)
    {
        TRACE_ERROR("ForceUseLocalServices registry key failed to open with EXCEPTION: %s", e.what());
    }

    TRACE_VERBOSE("IsForceUseLocalServicesEnabled: false");
    return false;
}

// Checks whether the Windows Store package is installed. If the Store is present,
// GRTS can be installed/updated, so out-of-proc mode is viable.
// Uses dynamic loading of GetPackagesByPackageFamily for Proton/Wine compatibility.
// Mirrors the detection logic in gamecore.grts.runtime (shared\api\lib\main.cpp).
bool IsWindowsStoreInstalled() noexcept
{
    const FARPROC pGetPackagesByPackageFamily = LoadGetPackagesByPackageFamilyFn();
    if (pGetPackagesByPackageFamily == nullptr)
    {
        // API not available (e.g., Proton/Wine without appmodel support).
        TRACE_VERBOSE("IsWindowsStoreInstalled: GetPackagesByPackageFamily not available");
        return false;
    }

    const auto GetPackagesByPackageFamilyFn =
        reinterpret_cast<PFN_GetPackagesByPackageFamily>(pGetPackagesByPackageFamily);

    constexpr PCWSTR c_storePackageFamilyName = L"Microsoft.WindowsStore_8wekyb3d8bbwe";

    uint32_t packageCount = 0;
    uint32_t bufferLen = 0;
    const LONG result = (*GetPackagesByPackageFamilyFn)(
        c_storePackageFamilyName,
        &packageCount,
        nullptr,
        &bufferLen,
        nullptr);

    // Interpret the result code per GetPackagesByPackageFamily docs:
    // - ERROR_INSUFFICIENT_BUFFER => at least one package exists (Store installed)
    // - APPMODEL_ERROR_NO_PACKAGE (or ERROR_SUCCESS with 0 packages) => not installed
    if (result == ERROR_INSUFFICIENT_BUFFER)
    {
        TRACE_VERBOSE(
            "IsWindowsStoreInstalled: true (result=%ld, packageCount=%u, bufferLen=%u)",
            result,
            packageCount,
            bufferLen);
        return true;
    }

    if (result == APPMODEL_ERROR_NO_PACKAGE || (result == ERROR_SUCCESS && packageCount == 0))
    {
        TRACE_VERBOSE("IsWindowsStoreInstalled: false (result=%ld)", result);
        return false;
    }

    // Unexpected failure (access denied, invalid parameters, etc.). Be conservative.
    TRACE_VERBOSE(
        "IsWindowsStoreInstalled: GetPackagesByPackageFamily failed (result=%ld, packageCount=%u, bufferLen=%u)",
        result,
        packageCount,
        bufferLen);
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
        // GRTS is available if the regkey exists OR the Windows Store is installed
        const bool regkeyAvailable = IsOutOfProcGRTSAvailable();
        const bool storeInstalled = IsWindowsStoreInstalled();
        const bool grtsAvailable = regkeyAvailable || storeInstalled;
        TRACE_VERBOSE("PlatformGetPlatformType: regkey=%s, store=%s -> grtsAvailable=%s",
            regkeyAvailable ? "true" : "false",
            storeInstalled ? "true" : "false",
            grtsAvailable ? "true" : "false");

        if (!grtsAvailable)
        {
            TRACE_INFORMATION("PlatformGetPlatformType: GRTS not available -> using defaults (Windows, None)");
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
            TRACE_INFORMATION("PlatformGetPlatformType: GRTS available -> platformInfo=GRTSAvailable");
        }
    }

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

    return S_OK;
}

}
