// Copyright (C) Microsoft Corporation. All rights reserved.
// DeviceGameSaveState_Win32.cpp - Windows-specific platform detection
#include "pch.h"
#include "Common/DeviceGameSaveState.h"
#include "Common/DeviceLogging.h"

#include <Windows.h>

#if HC_PLATFORM == HC_PLATFORM_GDK
#if defined(__has_include)
#if __has_include(<XSystem.h>)
#include <XSystem.h>
#define SAMPLE_HAS_XSYSTEM 1
#else
#define SAMPLE_HAS_XSYSTEM 0
#endif
#else
#include <XSystem.h>
#define SAMPLE_HAS_XSYSTEM 1
#endif
#else
#define SAMPLE_HAS_XSYSTEM 0
#endif

static DeviceGameSaveState g_deviceGameSaveState;

namespace
{
    constexpr wchar_t kGrtsSettingsKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OneSettings\\xbox\\GamingRuntimeServices\\Settings\\";
    constexpr wchar_t kGamingServicesKey[] = L"SOFTWARE\\Microsoft\\GamingServices";
    constexpr wchar_t kForceUseLocalServicesValue[] = L"ForceUseLocalServices";
    constexpr wchar_t kForceUseInprocGameSavesValue[] = L"ForceUseInprocGameSaves";

    bool IsRunningOnXboxConsole()
    {
#if SAMPLE_HAS_XSYSTEM
        auto deviceType = XSystemGetDeviceType();

        return deviceType == XSystemDeviceType::XboxOne ||
            deviceType == XSystemDeviceType::XboxOneS ||
            deviceType == XSystemDeviceType::XboxOneX ||
            deviceType == XSystemDeviceType::XboxOneXDevkit ||
            deviceType == XSystemDeviceType::XboxScarlettLockhart ||
            deviceType == XSystemDeviceType::XboxScarlettAnaconda ||
            deviceType == XSystemDeviceType::XboxScarlettDevkit;
#else
        return false;
#endif
    }

    bool IsOutOfProcGRTSAvailable()
    {
        HKEY hKey = nullptr;
        LONG lResult = ERROR_SUCCESS;

        try
        {
            lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGrtsSettingsKey, 0, KEY_READ, &hKey);
        }
        catch (const std::exception& e)
        {
            LogToWindowFormat("IsOutOfProcGRTSAvailable: exception while opening key (%s)", e.what());
            return false;
        }

        if (lResult != ERROR_SUCCESS)
        {
            if (lResult != ERROR_FILE_NOT_FOUND)
            {
                LogToWindowFormat("IsOutOfProcGRTSAvailable: RegOpenKeyEx failed (%ld)", lResult);
            }
            else
            {
                LogToWindow("IsOutOfProcGRTSAvailable: key not found -> false");
            }
            return false;
        }

        RegCloseKey(hKey);
        LogToWindow("IsOutOfProcGRTSAvailable: true");
        return true;
    }

    bool IsForceUseLocalServicesEnabled()
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
                    LogToWindow("IsForceUseLocalServicesEnabled: true");
                    return true;
                }
            }
        }
        catch (const std::exception& e)
        {
            LogToWindowFormat("IsForceUseLocalServicesEnabled: exception while querying key (%s)", e.what());
        }

        if (hKey != nullptr)
        {
            RegCloseKey(hKey);
        }

        LogToWindow("IsForceUseLocalServicesEnabled: false");
        return false;
    }

    bool IsForceUseInprocGameSavesRegkeySet()
    {
        HKEY hKey = nullptr;
        LONG lResult = ERROR_SUCCESS;

        try
        {
            DWORD forceUseInprocGameSaves = 0;
            DWORD dataSize = sizeof(DWORD);
            lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, kGamingServicesKey, 0, KEY_READ, &hKey);

            if (lResult == ERROR_SUCCESS)
            {
                lResult = RegQueryValueEx(hKey, kForceUseInprocGameSavesValue, nullptr, nullptr, reinterpret_cast<LPBYTE>(&forceUseInprocGameSaves), &dataSize);
                if (lResult == ERROR_SUCCESS && forceUseInprocGameSaves == 1)
                {
                    RegCloseKey(hKey);
                    LogToWindow("IsForceUseInprocGameSavesRegkeySet: true");
                    return true;
                }
            }
        }
        catch (const std::exception& e)
        {
            LogToWindowFormat("IsForceUseInprocGameSavesRegkeySet: exception while querying key (%s)", e.what());
        }

        if (hKey != nullptr)
        {
            RegCloseKey(hKey);
        }

        LogToWindow("IsForceUseInprocGameSavesRegkeySet: false");
        return false;
    }
}

DeviceGameSaveState* GetSampleGameSaveState()
{
    return &g_deviceGameSaveState;
}

void SetSampleDeviceEngineType(DeviceEngineType engineType)
{
    g_deviceGameSaveState.engineType = engineType;
}

DeviceEngineType DetectSampleDeviceEngineType()
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    if (IsRunningOnXboxConsole())
    {
        LogToWindow("DetectSampleDeviceEngineType: selecting Xbox engine");
        return DeviceEngineType::Xbox;
    }

    const bool outOfProcAvailable = IsOutOfProcGRTSAvailable();
    const bool forceLocalServices = IsForceUseLocalServicesEnabled();

    if (!outOfProcAvailable || forceLocalServices)
    {
        LogToWindow("DetectSampleDeviceEngineType: selecting in-proc engine");
        return DeviceEngineType::PcInproc;
    }

    const bool forceInprocGameSaves = IsForceUseInprocGameSavesRegkeySet();
    if (forceInprocGameSaves)
    {
        LogToWindow("DetectSampleDeviceEngineType: selecting in-proc game saves engine");
        return DeviceEngineType::PcInprocGameSaves;
    }

    LogToWindow("DetectSampleDeviceEngineType: selecting GRTS engine");
    return DeviceEngineType::PcGrts;
#else
    LogToWindow("DetectSampleDeviceEngineType: selecting Unknown engine (non-GDK)");
    return DeviceEngineType::Unknown;
#endif
}
