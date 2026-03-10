#include "stdafx.h"
#include "Platform/Platform.h"
#include "Platform/Windows/PFGameSaveFilesAPIProvider_GRTS.h"
#include "Platform/Windows/PFGameSaveFilesAPIProvider_Win32.h"

namespace PlayFab
{

namespace GameSave
{

bool IsForceUseInprocGameSavesRegkeySet()
{
    HKEY hKey = nullptr;
    LONG lResult;

    try
    {
        DWORD forceUseInprocGameSaves = 0;
        DWORD dataSize = sizeof(DWORD);
        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\GamingServices", 0, KEY_READ, &hKey);
        
        if (lResult == ERROR_SUCCESS)
        {
            lResult = RegQueryValueEx(hKey, L"ForceUseInprocGameSaves", nullptr, nullptr, reinterpret_cast<LPBYTE>(&forceUseInprocGameSaves), &dataSize);           
            if (lResult == ERROR_SUCCESS && forceUseInprocGameSaves == 1)
            {
                RegCloseKey(hKey);
                TRACE_INFORMATION("IsForceUseInprocGameSavesRegkeySet: true");
                return true;
            }
        }
    }
    catch (const std::exception& e)
    {
        TRACE_ERROR("forceUseInprocGameSaves registry key failed to open with EXCEPTION: %s", e.what());
    }
    
    if (hKey != nullptr)
    {
        RegCloseKey(hKey);
    }
    TRACE_VERBOSE("IsForceUseInprocGameSavesRegkeySet: false");
    return false;
}    

UniquePtr<GameSaveAPIProvider> PlatformGetAPIProvider(bool forceInproc) noexcept
{
    bool isGRTSAvailable;
    PFPlatformIsGRTSAvailable(&isGRTSAvailable);
    TRACE_VERBOSE("PlatformGetAPIProvider: PFPlatformIsGRTSAvailable -> %s", isGRTSAvailable ? "true" : "false");

    if (forceInproc)
    {
        isGRTSAvailable = false;
        TRACE_INFORMATION("PlatformGetAPIProvider: forceInproc parameter set -> forcing in-proc (Win32 provider)");
    }

    if (IsForceUseInprocGameSavesRegkeySet())
    {
        isGRTSAvailable = false;
        TRACE_INFORMATION("PlatformGetAPIProvider: ForceUseInprocGameSaves registry set -> forcing in-proc (Win32 provider)");
    }

    if (isGRTSAvailable)
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