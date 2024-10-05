#include "TestAppPch.h"
#include "Platform/PlatformUtils.h"
#include <chrono>
#include <thread>
#include <unistd.h>

namespace PlayFab
{
namespace Test
{
namespace Platform
{

String GetExeDirectory()
{
    char path[256] = {0};
    int size = 256;
    ssize_t len = readlink("/proc/self/exe", path, size - 1); // Linux
    if (len == -1) 
    {
        len = readlink("/proc/curproc/file", path, size - 1); // FreeBSD
        if (len == -1) 
        {
            len = readlink("/proc/self/path/a.out", path, size - 1); // Solaris
        }
    }

    if (len != -1) 
    {
        path[len] = '\0'; // Null-terminate the string
    }

    return String(path);
}


std::vector<char> readFileIntoVector(const std::string &filename) 
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::streampos fileSize = file.tellg();
    if (fileSize > 10000) {
        std::cerr << "File too large" << std::endl;
        exit(EXIT_FAILURE);
    }

    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    if (!file.read(buffer.data(), fileSize)) {
        std::cerr << "Error reading file" << std::endl;
        exit(EXIT_FAILURE);
    }

    file.close();
    return buffer;
}

HRESULT GetTestTitleData(TestTitleData& testTitleData) noexcept
{
     auto fileData = readFileIntoVector("testTitleData.json");

    // Parse JSON string into output TestTitleData.
    rapidjson::Document titleDataJson;
    titleDataJson.Parse(fileData.data());

    if (titleDataJson.HasParseError())
    {
        TraceMessage(HCTraceLevel::Error, "Unable to parse testTitleData.json");
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
    auto t = GetSystemTime();
    auto tm = std::localtime(&t);

    Stringstream ss;
    ss << std::put_time(tm, "%d-%m-%Y %H-%M-%S");

    return ss.str();
}

void WriteLogToFile(const char* line, const char* strFileName)
{
   FILE *file = fopen(strFileName, "a"); // "a" for append mode

    if (file == NULL) {
        std::cout << "Error opening file";
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", line);
    fclose(file);
}

void TraceMessage(HCTraceLevel level, String const& message) noexcept
{
    // Trace all messages to error stream
    std::cerr << message << std::endl;

    // // If HCTraceLevel <= Important direct to PFTestAppSummary.txt
    if (level <= HCTraceLevel::Important)
    {
        WriteLogToFile(message.data(), "PFTestAppSummary.txt");
    }

    // Trace all messages to PFTestAppEventsLog.txt
    WriteLogToFile(message.data(), "PFTestAppEventsLog.txt");

    if (level <= HCTraceLevel::Important)
    {
        std::cout << message;
    }
}

void Sleep(uint64_t durationMs) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
}

}
}
}
