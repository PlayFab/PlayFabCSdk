#include <windows.h>
#include "stdafx.h"
#include "Metadata.h"

namespace PlayFab
{
namespace GameSave
{
#if HC_PLATFORM == HC_PLATFORM_GDK

// Function pointer types for Windows API functions
typedef UINT (WINAPI *PFN_GetSystemDirectoryW)(LPWSTR lpBuffer, UINT uSize);
typedef DWORD (WINAPI *PFN_GetFileVersionInfoSizeExW)(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle);
typedef BOOL (WINAPI *PFN_GetFileVersionInfoExW)(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
typedef BOOL (WINAPI *PFN_VerQueryValueA)(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);
typedef BOOL (WINAPI *PFN_GetComputerNameA)(LPSTR lpBuffer, LPDWORD nSize);

// Struct to hold function pointers
struct WindowsApiFunctions
{
    PFN_GetSystemDirectoryW pfnGetSystemDirectoryW;
    PFN_GetFileVersionInfoSizeExW pfnGetFileVersionInfoSizeExW;
    PFN_GetFileVersionInfoExW pfnGetFileVersionInfoExW;
    PFN_VerQueryValueA pfnVerQueryValueA;
    PFN_GetComputerNameA pfnGetComputerNameA;
    HMODULE hKernel32;
    HMODULE hVersion;
    
    WindowsApiFunctions() : 
        pfnGetSystemDirectoryW(nullptr),
        pfnGetFileVersionInfoSizeExW(nullptr),
        pfnGetFileVersionInfoExW(nullptr),
        pfnVerQueryValueA(nullptr),
        pfnGetComputerNameA(nullptr),
        hKernel32(nullptr),
        hVersion(nullptr)
    {
    }
    
    ~WindowsApiFunctions()
    {
        if (hKernel32)
        {
            FreeLibrary(hKernel32);
            hKernel32 = nullptr;
        }
        if (hVersion)
        {
            FreeLibrary(hVersion);
            hVersion = nullptr;
        }
    }
};

// Initialize function pointers
static void InitializeFunctionPointers(WindowsApiFunctions& apiFunctions)
{
    if (apiFunctions.pfnGetSystemDirectoryW == nullptr)
    {
        apiFunctions.hKernel32 = LoadLibraryW(L"kernel32.dll");
        if (apiFunctions.hKernel32)
        {
            apiFunctions.pfnGetSystemDirectoryW = (PFN_GetSystemDirectoryW)GetProcAddress(apiFunctions.hKernel32, "GetSystemDirectoryW");
            apiFunctions.pfnGetComputerNameA = (PFN_GetComputerNameA)GetProcAddress(apiFunctions.hKernel32, "GetComputerNameA");
        }
    }

    if (apiFunctions.pfnGetFileVersionInfoSizeExW == nullptr)
    {
        apiFunctions.hVersion = LoadLibraryW(L"version.dll");
        if (apiFunctions.hVersion)
        {
            apiFunctions.pfnGetFileVersionInfoSizeExW = (PFN_GetFileVersionInfoSizeExW)GetProcAddress(apiFunctions.hVersion, "GetFileVersionInfoSizeExW");
            apiFunctions.pfnGetFileVersionInfoExW = (PFN_GetFileVersionInfoExW)GetProcAddress(apiFunctions.hVersion, "GetFileVersionInfoExW");
            apiFunctions.pfnVerQueryValueA = (PFN_VerQueryValueA)GetProcAddress(apiFunctions.hVersion, "VerQueryValueA");
        }
    }
}
String GetDeviceType()
{
    return "Windows";
}
 

String GetDeviceVersion()
{
    // Initialize function pointers
    WindowsApiFunctions apiFunctions;
    InitializeFunctionPointers(apiFunctions);
    
    // GetVersionEx is deprecated, so just report the file version of kernel32.dll
    WCHAR systemDir[MAX_PATH];
    UINT result = 0;
    
    if (apiFunctions.pfnGetSystemDirectoryW)
    {
        result = apiFunctions.pfnGetSystemDirectoryW(systemDir, MAX_PATH);
    }
    
    if (result != 0 && result < MAX_PATH)
    {
        WString filePath = systemDir;
        filePath += L"\\kernel32.dll";

        DWORD size = 0;
        if (apiFunctions.pfnGetFileVersionInfoSizeExW)
        {
            size = apiFunctions.pfnGetFileVersionInfoSizeExW(FILE_VER_GET_LOCALISED, filePath.c_str(), nullptr);
        }
        
        if (size != 0)
        {
            std::vector<BYTE> buffer(size);
            if (apiFunctions.pfnGetFileVersionInfoExW && apiFunctions.pfnGetFileVersionInfoExW(FILE_VER_GET_LOCALISED, filePath.c_str(), 0, size, buffer.data()))
            {
                VS_FIXEDFILEINFO* fileInfo = nullptr;
                UINT fileInfoLen = 0;
                if (apiFunctions.pfnVerQueryValueA && apiFunctions.pfnVerQueryValueA(buffer.data(), "\\", reinterpret_cast<LPVOID*>(&fileInfo), &fileInfoLen))
                {
                    if (fileInfoLen != 0 && fileInfo != nullptr)
                    {
                        Stringstream versionStream;
                        versionStream << HIWORD(fileInfo->dwProductVersionMS) << '.'
                            << LOWORD(fileInfo->dwProductVersionMS) << '.'
                            << HIWORD(fileInfo->dwProductVersionLS) << '.'
                            << LOWORD(fileInfo->dwProductVersionLS);
                        return versionStream.str();
                    }
                }
            }
        }
    }

    // If we can't get the version, just return a default value
    return "10.0.0.0";
}

String GetDeviceFriendlyName()
{
    // Initialize function pointers
    WindowsApiFunctions apiFunctions;
    InitializeFunctionPointers(apiFunctions);
    
    char name[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
    BOOL success = FALSE;
    
    if (apiFunctions.pfnGetComputerNameA)
    {
        success = apiFunctions.pfnGetComputerNameA(name, &dwSize);
    }
    
    if (success)
    {
        return String(name);
    }
    else
    {
        // If we can't get the computer name, just return blank
        return String("");
    }
}

#endif
} // namespace GameSave
} // namespace PlayFab