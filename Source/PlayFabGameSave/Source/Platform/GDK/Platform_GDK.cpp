#include "stdafx.h"
#include "Platform/Platform.h"
#include "Platform/Windows/PFGameSaveFilesAPIProvider_GRTS.h"
#include "Platform/Windows/PFGameSaveFilesAPIProvider_Win32.h"

namespace PlayFab
{

namespace GameSave
{

// Signatures and constants for APIs that are dynamically loaded below.
// We declare these explicitly rather than relying on <winreg.h>/<appmodel.h>
// symbol visibility, because:
//   1. httpClient/pal.h includes <windows.h> with WIN32_LEAN_AND_MEAN, which
//      under the GDK SDK omits winreg.h declarations.
//   2. WINAPI_FAMILY_GAMES partitions can gate <appmodel.h> declarations off.
//   3. The dynamic-loading approach intentionally avoids any compile-time
//      dependency on the symbols (Proton/Wine, older kernel32 don't export them).
using PFN_RegGetValueW = LONG (WINAPI*)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);

#ifndef RRF_RT_REG_DWORD
#define RRF_RT_REG_DWORD 0x00000010
#endif

// Dynamic loading for RegGetValueW — may not be available on Proton/Wine or older kernel32.
// Mirrors the approach in gamecore.grts.runtime (shared\api\lib\main.cpp).
static FARPROC LoadRegGetValueWFn() noexcept
{
    static HMODULE s_advapi32Dll = []() noexcept -> HMODULE
    {
        return LoadLibraryExW(L"advapi32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }();

    if (s_advapi32Dll == nullptr)
    {
        return nullptr;
    }

    return GetProcAddress(s_advapi32Dll, "RegGetValueW");
}

// RAII wrapper for Windows registry key handles
class RegistryKeyHandle
{
public:
    RegistryKeyHandle() noexcept : m_hKey(nullptr) {}
    ~RegistryKeyHandle() noexcept
    {
        if (m_hKey != nullptr)
        {
            RegCloseKey(m_hKey);
        }
    }

    // Non-copyable
    RegistryKeyHandle(const RegistryKeyHandle&) = delete;
    RegistryKeyHandle& operator=(const RegistryKeyHandle&) = delete;

    HKEY* AddressOf() noexcept { return &m_hKey; }
    HKEY Get() const noexcept { return m_hKey; }
    bool IsValid() const noexcept { return m_hKey != nullptr; }

private:
    HKEY m_hKey;
};

bool IsForceUseInprocGameSavesRegkeySet() noexcept
{
    RegistryKeyHandle hKey;
    DWORD forceUseInprocGameSaves = 0;
    DWORD dataSize = sizeof(DWORD);

    LONG lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\GamingServices", 0, KEY_READ, hKey.AddressOf());
    if (lResult != ERROR_SUCCESS)
    {
        TRACE_VERBOSE("IsForceUseInprocGameSavesRegkeySet: RegOpenKeyEx failed with error %ld", lResult);
        return false;
    }

    lResult = RegQueryValueExW(hKey.Get(), L"ForceUseInprocGameSaves", nullptr, nullptr, 
        reinterpret_cast<LPBYTE>(&forceUseInprocGameSaves), &dataSize);
    if (lResult == ERROR_SUCCESS && forceUseInprocGameSaves == 1)
    {
        TRACE_INFORMATION("IsForceUseInprocGameSavesRegkeySet: true");
        return true;
    }

    TRACE_VERBOSE("IsForceUseInprocGameSavesRegkeySet: false");
    return false;
}

// Checks HKLM\Software\Microsoft\GamingServices\ForceUseLocalServices (DWORD).
// This is the registry override used by the GRTS runtime to force inproc mode.
// Uses dynamic loading of RegGetValueW to avoid advapi32 link dependency on Proton/Wine.
bool IsForceUseLocalServicesRegkeySet() noexcept
{
    const FARPROC pRegGetValueW = LoadRegGetValueWFn();
    if (pRegGetValueW == nullptr)
    {
        TRACE_VERBOSE("IsForceUseLocalServicesRegkeySet: RegGetValueW not available");
        return false;
    }

    const auto RegGetValueWFn = reinterpret_cast<PFN_RegGetValueW>(pRegGetValueW);

    DWORD configValue = 0;
    DWORD configSize = sizeof(configValue);

    if ((*RegGetValueWFn)(
        HKEY_LOCAL_MACHINE,
        L"Software\\Microsoft\\GamingServices",
        L"ForceUseLocalServices",
        RRF_RT_REG_DWORD,
        nullptr,
        &configValue,
        &configSize) == ERROR_SUCCESS)
    {
        if (configValue != 0)
        {
            TRACE_INFORMATION("IsForceUseLocalServicesRegkeySet: true");
            return true;
        }
    }

    TRACE_VERBOSE("IsForceUseLocalServicesRegkeySet: false");
    return false;
}

UniquePtr<GameSaveAPIProvider> PlatformGetAPIProvider(bool forceInproc) noexcept
{
    // PFCore's PlatformGetPlatformType already combines regkey + Store checks
    bool useGRTS;
    PFPlatformIsGRTSAvailable(&useGRTS);
    TRACE_VERBOSE("PlatformGetAPIProvider: PFPlatformIsGRTSAvailable -> %s", useGRTS ? "true" : "false");

    // Override: forceInproc parameter
    if (forceInproc)
    {
        useGRTS = false;
        TRACE_INFORMATION("PlatformGetAPIProvider: forceInproc parameter set -> forcing in-proc (Win32 provider)");
    }

    // Override: ForceUseInprocGameSaves registry key
    if (IsForceUseInprocGameSavesRegkeySet())
    {
        useGRTS = false;
        TRACE_INFORMATION("PlatformGetAPIProvider: ForceUseInprocGameSaves registry set -> forcing in-proc (Win32 provider)");
    }

    // Override: ForceUseLocalServices registry key (GRTS runtime convention)
    if (IsForceUseLocalServicesRegkeySet())
    {
        useGRTS = false;
        TRACE_INFORMATION("PlatformGetAPIProvider: ForceUseLocalServices registry set -> forcing in-proc (Win32 provider)");
    }

    if (useGRTS)
    {
        TRACE_INFORMATION("PlatformGetAPIProvider: selecting GRTS provider");
        return UniquePtr<GameSaveAPIProvider>(MakeUnique<GameSaveAPIProviderGRTS>().release());
    }
    else
    {
        TRACE_INFORMATION("PlatformGetAPIProvider: selecting Win32 provider");
        return UniquePtr<GameSaveAPIProvider>(MakeUnique<GameSaveAPIProviderWin32>().release());
    }
}

} // namespace GameSave
} // namespace PlayFab