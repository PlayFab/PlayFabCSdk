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
    wchar_t wname[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
    BOOL success = GetComputerNameW(wname, &dwSize);
    if (success)
    {
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wname, -1, nullptr, 0, nullptr, nullptr);
        if (utf8Len > 0)
        {
            String result(static_cast<size_t>(utf8Len - 1), '\0');
            WideCharToMultiByte(CP_UTF8, 0, wname, -1, &result[0], utf8Len, nullptr, nullptr);
            return result;
        }
    }
    return String("");
}
#endif

} // namespace GameSave
} // namespace PlayFab