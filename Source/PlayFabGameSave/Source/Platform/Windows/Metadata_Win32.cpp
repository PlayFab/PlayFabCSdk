#include "stdafx.h"
#include "Metadata.h"
#include <windows.h>

namespace PlayFab
{
namespace GameSave
{

#if defined(_WIN32) && HC_PLATFORM != HC_PLATFORM_GDK
String GetDeviceType()
{
    return "Windows";
}

String GetDeviceVersion()
{
    // GetVersionEx is deprecated, so just report the file version of kernel32.dll
    WCHAR systemDir[MAX_PATH];
    UINT result = GetSystemDirectoryW(systemDir, MAX_PATH);
    if (result != 0 && result < MAX_PATH)
    {
        WString filePath = systemDir;
        filePath += L"\\kernel32.dll";

        DWORD size = GetFileVersionInfoSizeExW(FILE_VER_GET_LOCALISED, filePath.c_str(), nullptr);
        if (size != 0)
        {
            std::vector<BYTE> buffer(size);
            if (GetFileVersionInfoExW(FILE_VER_GET_LOCALISED, filePath.c_str(), 0, size, buffer.data()))
            {
                VS_FIXEDFILEINFO* fileInfo = nullptr;
                UINT fileInfoLen = 0;
                if (VerQueryValueA(buffer.data(), "\\", reinterpret_cast<LPVOID*>(&fileInfo), &fileInfoLen))
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
    char name[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
    BOOL success = GetComputerNameA(name, &dwSize);
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