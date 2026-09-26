#include "pch.h"
#include "FilePAL.h"
#include <filesystem>

namespace PlayFab
{

FILETIME TimeTToFileTime(time_t t) noexcept
{
    ULARGE_INTEGER ull{};
    ull.QuadPart = Int32x32To64(t, 10000000) + 116444736000000000;
    FILETIME fileTime{};
    fileTime.dwLowDateTime = ull.LowPart;
    fileTime.dwHighDateTime = ull.HighPart;
    return fileTime;
}

std::wstring ConvertToWString(const String& utf8_string)
{
    if (utf8_string.empty()) return std::wstring();

    if (utf8_string.size() > static_cast<size_t>(INT_MAX)) return std::wstring();

    // Calculate the required length for the UTF-16 string
    int wstr_length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), (int)utf8_string.size(), nullptr, 0);
    if (wstr_length == 0) return std::wstring();

    std::wstring wstr_string(wstr_length, 0);

    int result = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), (int)utf8_string.size(), reinterpret_cast<LPWSTR>(&wstr_string[0]), wstr_length);
    if (result == 0) return std::wstring();

    return wstr_string;
}

std::filesystem::path ConvertStringToPath(const String& pathUtf8)
{
    return std::filesystem::path(ConvertToWString(pathUtf8));
}

HRESULT FilePAL::SetFileLastModifiedTime(const String& filePath, time_t timeCreated, time_t timeModified) noexcept
{
    auto path = ConvertToWString(filePath);

    HANDLE hFile = CreateFileW(path.c_str(), 
        FILE_WRITE_ATTRIBUTES, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS, 
        NULL);

    // Not an argument error: the file can be missing, denied, or held by another writer. Report
    // what actually happened so callers can tell a transient condition from a permanent one.
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    FILETIME ftNewCreationTime = TimeTToFileTime(timeCreated);
    FILETIME ftNewModifiedTime = TimeTToFileTime(timeModified);

    if (!SetFileTime(hFile, &ftNewCreationTime, &ftNewModifiedTime, &ftNewModifiedTime))
    {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        CloseHandle(hFile);
        return hr;
    }

    CloseHandle(hFile);
    return S_OK;
}

} // namespace FileIO