#include "TestAppPch.h"
#include "Platform/PlatformUtils.h"
#if defined(_GAMING_DESKTOP)
#include <shlobj.h>
#endif

namespace PlayFab
{
namespace Test
{
namespace Platform
{

String GetExeDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    String::size_type pos = String(path).find_last_of("\\/");
    return String(path).substr(0, pos);
}

HRESULT GetTestTitleData(TestTitleData& testTitleData) noexcept
{
    // Read the title data from the file into the output character array.
    std::ifstream titleDataFile;
    SetCurrentDirectoryA(GetExeDirectory().c_str());

    std::ios_base::iostate exceptionMask = titleDataFile.exceptions() | std::ios::failbit;
    titleDataFile.exceptions(exceptionMask);

    titleDataFile.open("testTitleData.json", std::ios::in | std::ios::ate);
    if (!titleDataFile.is_open())
    {
        return E_FAIL;
    }

    int64_t size = titleDataFile.tellg();
    if (size < 0)
    {
        return E_FAIL;
    }

    titleDataFile.seekg(0);

    Vector<char> data(static_cast<size_t>(size + 1), 0);
    titleDataFile.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(size));
    if (!titleDataFile.good())
    {
        return E_FAIL;
    }

    titleDataFile.close();

    // Parse JSON string into output TestTitleData.
    rapidjson::Document titleDataJson;
    titleDataJson.Parse(data.data());

    if (titleDataJson.HasParseError())
    {
        TraceMessage(PFHCTraceLevel::Error, "Unable to parse testTitleData.json");
        return E_FAIL;
    }

    testTitleData.titleId = titleDataJson["titleId"].GetString();
    testTitleData.secretKey = titleDataJson["secretKey"].GetString();
    testTitleData.connectionString = titleDataJson["connectionString"].GetString();

    return S_OK;
}

time_t GetSystemTime() noexcept
{
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

String GetTimeString() noexcept
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    char szTime[256]{ 0 };
    sprintf_s(szTime, 255, "[%0.2d/%0.2d %0.2d:%0.2d:%0.2d:%0.4d] ", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return szTime;
}

void WriteLogToFile(const char* strIn, const char* strFileName)
{
    HANDLE hFile;
    std::string str = strIn;
    DWORD dwBytesToWrite = (DWORD)str.length();
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

    static bool firstLogLine = true;
    DWORD dwCreationDisposition = OPEN_ALWAYS;
    if (firstLogLine)
    {
        firstLogLine = false;
        dwCreationDisposition = CREATE_ALWAYS; // recreate log upon start
    }

#if defined(_GAMING_DESKTOP)
    char szPath[MAX_PATH];
    SHGetSpecialFolderPathA(HWND_DESKTOP, szPath, CSIDL_DESKTOP, FALSE);
    std::string strFullPath = szPath;
#else
    std::string strFullPath = "D:";
#endif
    strFullPath += "\\";
    strFullPath += strFileName;
    hFile = CreateFileA(strFullPath.c_str(), FILE_APPEND_DATA, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    bErrorFlag = WriteFile(
        hFile,           // open file handle
        str.data(),      // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (!bErrorFlag && dwBytesWritten != dwBytesToWrite)
    {
        OutputDebugStringA("Log file error: dwBytesWritten != dwBytesToWrite\n");
    }

    CloseHandle(hFile);
}

void TraceMessage(PFHCTraceLevel level, String const& message) noexcept
{
    // Trace all messages to debugger output
    OutputDebugStringA(message.data());

    // If PFHCTraceLevel <= Important direct to PFTestAppSummary.txt
    if (level <= PFHCTraceLevel::Important)
    {
        WriteLogToFile(message.data(), "PFTestAppSummary.txt");
    }

    // Trace all messages to PFTestAppEventsLog.txt
    WriteLogToFile(message.data(), "PFTestAppEventsLog.txt");

#if HC_PLATFORM == HC_PLATFORM_WIN32
    // On Win32 also redirect to STDOUT
    std::cout << message;
#endif
}

void Sleep(uint64_t durationMs) noexcept
{
    ::Sleep((DWORD)durationMs);
}

}
}
}