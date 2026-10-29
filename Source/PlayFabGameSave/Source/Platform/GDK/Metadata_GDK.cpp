#include "stdafx.h"
#include <windows.h>
#include <limits>
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
typedef BOOL (WINAPI *PFN_GetComputerNameW)(LPWSTR lpBuffer, LPDWORD nSize);

// Struct to hold function pointers
struct WindowsApiFunctions
{
    PFN_GetSystemDirectoryW pfnGetSystemDirectoryW;
    PFN_GetFileVersionInfoSizeExW pfnGetFileVersionInfoSizeExW;
    PFN_GetFileVersionInfoExW pfnGetFileVersionInfoExW;
    PFN_VerQueryValueA pfnVerQueryValueA;
    PFN_GetComputerNameW pfnGetComputerNameW;
    HMODULE hKernel32;
    HMODULE hVersion;
    
    WindowsApiFunctions() : 
        pfnGetSystemDirectoryW(nullptr),
        pfnGetFileVersionInfoSizeExW(nullptr),
        pfnGetFileVersionInfoExW(nullptr),
        pfnVerQueryValueA(nullptr),
        pfnGetComputerNameW(nullptr),
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
            apiFunctions.pfnGetComputerNameW = (PFN_GetComputerNameW)GetProcAddress(apiFunctions.hKernel32, "GetComputerNameW");
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
// SteamOS detection. Under Proton/Wine the kernel32 file version is Wine's emulated Windows
// version, which is meaningless for a Steam Deck. Read the Linux host's os-release instead.
namespace
{

using PFN_wine_get_dos_file_name = WCHAR* (CDECL*)(LPCSTR unixPath);

bool IsRunningUnderWine() noexcept
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    return ntdll != nullptr && GetProcAddress(ntdll, "wine_get_version") != nullptr;
}

WString UnixPathToDosPath(const char* unixPath)
{
    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    if (kernel32 != nullptr)
    {
        auto pfnWineGetDosFileName = reinterpret_cast<PFN_wine_get_dos_file_name>(GetProcAddress(kernel32, "wine_get_dos_file_name"));
        if (pfnWineGetDosFileName != nullptr)
        {
            WCHAR* dosPath = pfnWineGetDosFileName(unixPath);
            if (dosPath != nullptr)
            {
                WString result{ dosPath };
                HeapFree(GetProcessHeap(), 0, dosPath);
                return result;
            }
        }
    }

    // Fallback: Wine/Proton prefixes map Z: to the Unix root by default
    WString result{ L"Z:" };
    for (const char* p = unixPath; *p != '\0'; ++p)
    {
        result += (*p == '/') ? L'\\' : static_cast<wchar_t>(static_cast<unsigned char>(*p));
    }
    return result;
}

String TrimWhitespace(const String& value)
{
    const char* whitespace = " \t\r\n";
    size_t start = value.find_first_not_of(whitespace);
    if (start == String::npos)
    {
        return String{};
    }
    size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

struct OsReleaseInfo
{
    String id;
    String versionId;
    String buildId;
};

bool TryReadOsRelease(const char* unixPath, OsReleaseInfo& info)
{
    info = OsReleaseInfo{};

    std::ifstream file(UnixPathToDosPath(unixPath).c_str());
    if (!file.is_open())
    {
        return false;
    }

    // os-release is external input; bound the read so a malformed file can't cause large allocations
    constexpr size_t maxLines = 128;
    char lineBuffer[512];
    for (size_t lineCount = 0; lineCount < maxLines; ++lineCount)
    {
        file.getline(lineBuffer, sizeof(lineBuffer));
        if (!file)
        {
            if (file.eof())
            {
                break;
            }

            // Line exceeded the buffer; skip the rest of it so later fields are still parsed
            file.clear();
            file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }

        String line = TrimWhitespace(lineBuffer);
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t separator = line.find('=');
        if (separator == String::npos)
        {
            continue;
        }

        String key = TrimWhitespace(line.substr(0, separator));
        String value = TrimWhitespace(line.substr(separator + 1));
        if (value.size() >= 2 && (value.front() == '"' || value.front() == '\'') && value.back() == value.front())
        {
            value = value.substr(1, value.size() - 2);
        }

        if (key == "ID")
        {
            info.id = value;
        }
        else if (key == "VERSION_ID")
        {
            info.versionId = value;
        }
        else if (key == "BUILD_ID")
        {
            info.buildId = value;
        }
    }

    return !info.id.empty();
}

// Version is uploaded as manifest DeviceVersion (service limit 100 chars); require short printable ASCII
bool IsValidVersionString(const String& value)
{
    constexpr size_t maxVersionLength = 32;
    if (value.empty() || value.size() > maxVersionLength)
    {
        return false;
    }
    return std::all_of(value.begin(), value.end(), [](char c)
    {
        unsigned char uc = static_cast<unsigned char>(c);
        return uc > 0x20 && uc < 0x7F;
    });
}

// Returns "SteamOS <version>" when running under Proton on SteamOS, otherwise empty.
String TryGetSteamOSVersion()
{
    if (!IsRunningUnderWine())
    {
        return String{};
    }

    // Inside the Steam Linux Runtime (pressure-vessel) container the host root is mounted at
    // /run/host, and /etc/os-release describes the container (ID=steamrt), so prefer host paths.
    static const char* const s_osReleasePaths[] =
    {
        "/run/host/etc/os-release",
        "/run/host/usr/lib/os-release",
        "/etc/os-release",
        "/usr/lib/os-release",
    };

    for (const char* path : s_osReleasePaths)
    {
        OsReleaseInfo info;
        if (!TryReadOsRelease(path, info) || info.id == "steamrt")
        {
            continue;
        }

        // First host os-release found is authoritative
        if (info.id != "steamos")
        {
            TRACE_VERBOSE("[GAME SAVE] TryGetSteamOSVersion: host OS is '%s', not SteamOS", info.id.c_str());
            return String{};
        }

        // Prefer VERSION_ID, but fall back to BUILD_ID if VERSION_ID is missing or invalid
        String version = IsValidVersionString(info.versionId) ? info.versionId
            : (IsValidVersionString(info.buildId) ? info.buildId : String{});
        String result = !version.empty() ? String{ "SteamOS " } + version : String{ "SteamOS" };
        TRACE_INFORMATION("[GAME SAVE] TryGetSteamOSVersion: detected '%s' via %s", result.c_str(), path);
        return result;
    }

    TRACE_VERBOSE("[GAME SAVE] TryGetSteamOSVersion: running under Wine but no host os-release found");
    return String{};
}

} // namespace

String GetDeviceType()
{
    return "Windows";
}
 

String GetDeviceVersion()
{
    String steamOSVersion = TryGetSteamOSVersion();
    if (!steamOSVersion.empty())
    {
        return steamOSVersion;
    }

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
    
    wchar_t wname[MAX_COMPUTERNAME_LENGTH + 1] = {};
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
    BOOL success = FALSE;
    
    if (apiFunctions.pfnGetComputerNameW)
    {
        success = apiFunctions.pfnGetComputerNameW(wname, &dwSize);
    }
    
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