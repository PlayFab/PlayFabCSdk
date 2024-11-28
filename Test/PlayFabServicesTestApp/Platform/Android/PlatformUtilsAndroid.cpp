#include "TestAppPch.h"
#include "PlatformUtilsAndroid.h"
#include "Platform/PlatformUtils.h"
#include "AndroidTestApp.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cassert>
#include <string.h>
#include "nlohmann/json.hpp"

JNIEnv* JniEnvFromJavaVm(JavaVM* javaVm)
{
    assert(javaVm);

    JNIEnv* jniEnv = nullptr;
    jint result = javaVm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);

    if (result != JNI_OK)
    {
        LOGE("Failed to retrieve the JNIEnv from the JavaVM.");
        return nullptr;
    }

    assert(jniEnv != nullptr);

    return jniEnv;
}

JavaVM* JavaVmFromJniEnv(JNIEnv* env)
{
    JavaVM* javaVM = nullptr;
    jint res = env->GetJavaVM(&javaVM);
    if (res != JNI_OK)
    {
        LOGE("Failed to retrieve the JavaVM from the provided Environment");
        return nullptr;
    }

    return javaVM;
}

namespace PlayFab
{
namespace Test
{
namespace Platform
{

HRESULT GetTestTitleData(TestTitleData& testTitleData) noexcept
{
    std::vector<char> fileBuffer;
    AndroidTestApp::TestApp::GetInstance().GetBufferFromFile("testTitleData.json", fileBuffer);

    // Parse JSON string into output TestTitleData.
    JsonValue titleDataJson;
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        if (fileBuffer.data())
        {
            titleDataJson = JsonValue::parse(fileBuffer.data());
        }
        else
        {
            parseError = true;
        }
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
    }

    if (parseError)
    {
        LOGE("Unable to parse testTitleData.json");
        LOGE("\nError: %s\n", parseErrorMsg.c_str());
        return E_FAIL;
    }

    testTitleData.titleId = titleDataJson["titleId"].get<String>();
    testTitleData.secretKey = titleDataJson["secretKey"].get<String>();
    testTitleData.connectionString = titleDataJson["connectionString"].get<String>();
    testTitleData.allowRetries = titleDataJson["allowRetries"].get<bool>();
    testTitleData.runTestList = titleDataJson["runTestList"].get<bool>();
    testTitleData.testList = titleDataJson["testList"].get<Set<String>>();
    testTitleData.retryableHRs = titleDataJson["retryableHRs"].get<Set<String>>();

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

void WriteLogToFile(const char* message)
{
    if (strlen(message) < 4000) {
        LOGI("%s", message);
        return ;
    }

    char* pch = strtok((char*) message, "\n");
    while (pch != NULL)
    {
        LOGI("%s", pch);
        pch = strtok(NULL, "\n");
    }
}

void TraceMessage(HCTraceLevel level, String const& message) noexcept
{
    // If PFHCTraceLevel <= Important direct to PFTestAppSummary.txt
    if (level <= HCTraceLevel::Important)
    {
        WriteLogToFile(message.c_str());
    }
}

void Sleep(uint64_t durationMs) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
}

}
}
}
