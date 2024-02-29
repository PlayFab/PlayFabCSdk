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

auto read_file(std::string_view path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream) {
        throw std::ios_base::failure("File does not exist");
    }
    
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

std::string readFileIntoVector()
{
    char * envVariableValue = getenv("TESTTITLEDATA");
    std::string filePath = envVariableValue == NULL ? std::string("") : std::string(envVariableValue);
    return read_file(filePath);
}

HRESULT GetTestTitleData(TestTitleData& testTitleData) noexcept
{
     auto fileData = readFileIntoVector();

    // Parse JSON string into output TestTitleData.
    rapidjson::Document titleDataJson;
    TraceMessage(PFHCTraceLevel::Verbose, fileData.data());
    titleDataJson.Parse(fileData.data());

    if (titleDataJson.HasParseError())
    {
        TraceMessage(PFHCTraceLevel::Error, "Unable to parse testTitleData.json");
        return E_FAIL;
    }

    testTitleData.titleId = titleDataJson["titleId"].GetString();
    testTitleData.secretKey = titleDataJson["secretKey"].GetString();
    testTitleData.connectionString = titleDataJson["connectionString"].GetString();
    
#if HC_PLATFORM == HC_PLATFORM_MAC
    testTitleData.steamAppId = titleDataJson["steamAppId"].GetString();
    testTitleData.steamPublisherKey = titleDataJson["steamPublisherKey"].GetString();
#endif
        
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
    char buffer[256];

    //HOME is the home directory of your application
    //points to the root of your sandbox
    strcpy(buffer,getenv("HOME"));

    //concatenating the path string returned from HOME
    strcat(buffer,"/Documents/");
    strcat(buffer,strFileName);
    
   FILE *file = fopen(buffer, "a"); // "a" for append mode

    if (file == NULL) {
        std::cout << "Error opening file";
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", line);
    fclose(file);
}

void TraceMessage(PFHCTraceLevel level, String const& message) noexcept
{
    // Trace all messages to error stream
    std::cerr << message << std::endl;

    // If PFHCTraceLevel <= Important direct to PFTestAppSummary.txt
    if (level <= PFHCTraceLevel::Important)
    {
        WriteLogToFile(message.data(), "PFTestAppSummary.txt");
    }

    // Trace all messages to PFTestAppEventsLog.txt
    WriteLogToFile(message.data(), "PFTestAppEventsLog.txt");

    if (level <= PFHCTraceLevel::Important)
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
