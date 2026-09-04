#include "TestAppPch.h"
#include "AndroidTestApp.h"
#include "PlatformUtilsAndroid.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Nlohmann.h"
#include "TestRunner.h"
#include "../PlatformUtils.h"

#include <chrono>
#include <future>
#include <memory>
#include <thread>

namespace AndroidTestApp
{

/* static */ TestApp& TestApp::GetInstance()
{
    static TestApp instance;
    return instance;
}

void TestApp::AppInitialize(
    JNIEnv* env,
    jobject activityInstance,
    jobject context,
    jstring currentPlayerId,
    jobject signInClient
)
{
    auto lock = Lock();
    if (m_initialized)
    {
        assert(false);
        return;
    }

    m_activityInstance = env->NewGlobalRef(activityInstance);
    m_context = env->NewGlobalRef(context);
    m_signInClient = env->NewGlobalRef(signInClient);

    char const* nativePlayerId = env->GetStringUTFChars(currentPlayerId, nullptr);
    m_currentPlayerId = nativePlayerId;
    env->ReleaseStringUTFChars(currentPlayerId, nativePlayerId);

    jclass testAppClass = env->FindClass("com/microsoft/playfab/sdk/AndroidTestClient");
    if (testAppClass == nullptr)
    {
        LOGE("Couldn't find AndroidTestClient class");
        return;
    }

    m_testAppClass = static_cast<jclass>(env->NewGlobalRef(testAppClass));

    m_javaVm = JavaVmFromJniEnv(env);
    assert(m_javaVm != nullptr);

    m_getBufferFromFileMethod = env->GetMethodID(
        m_testAppClass,
        "GetBufferFromFile",
        "(Ljava/lang/String;)[B"
    );

    m_getDeviceTokenMethod = env->GetMethodID(
        m_testAppClass,
        "GetDeviceToken",
        "()Ljava/lang/String;"
    );

    m_getServerAuthTokenMethod = env->GetMethodID(
        m_testAppClass,
        "GetServerAuthToken",
        "()Ljava/lang/String;"
    );

    HCTraceSetTraceToDebugger(true);

    LOGI("AndroidTestApp initialized");

    m_initialized = true;
}

void TestApp::UpdateInstances(JNIEnv* env, jobject activityInstance, jobject context)
{
    auto lock = Lock();
    if (!m_initialized)
    {
        assert(false);
        return;
    }

    assert(m_activityInstance);
    assert(m_context);

    env->DeleteGlobalRef(m_activityInstance);
    env->DeleteGlobalRef(m_context);

    m_activityInstance = env->NewGlobalRef(activityInstance);
    m_context = env->NewGlobalRef(context);
}

bool TestApp::RunTests()
{
    auto testRunner = std::make_shared<PlayFab::Test::TestRunner>();
    THROW_IF_FAILED(testRunner->Initialize());

    while (!testRunner->Update())
    {
        PlayFab::Test::Platform::Sleep(10);
    }

    bool const allTestsPassed = testRunner->AllTestsPassed();

    // Workaround for a known SDK cleanup wedge on the Android x86_64 emulator:
    // XTaskQueueTerminate's async completion callback (see
    // Source/PlayFabSharedInternal/Source/RunContext.cpp) occasionally never
    // fires. Run cleanup on a separate thread so the finalized test result can
    // still return through JUnit. Killing the process here disconnects the
    // instrumentation runner and leaves connectedAndroidTest waiting.
    constexpr auto kCleanupWatchdogTimeout = std::chrono::seconds(30);
    std::packaged_task<bool()> cleanupTask{ [testRunner]() {
        return testRunner->Cleanup();
    } };
    auto cleanupResult = cleanupTask.get_future();
    std::thread cleanupThread{ std::move(cleanupTask) };

    if (cleanupResult.wait_for(kCleanupWatchdogTimeout) == std::future_status::ready)
    {
        bool const cleanupPassed = cleanupResult.get();
        cleanupThread.join();
        return cleanupPassed;
    }

    LOGE(
        "testRunner.Cleanup() exceeded %lld s watchdog; returning finalized test result.",
        static_cast<long long>(kCleanupWatchdogTimeout.count())
    );
    cleanupThread.detach();
    return allTestsPassed;
}

TestApp::TestApp()
{
    m_initialized = false;
    m_javaVm = nullptr;
    m_context = nullptr;
    m_activityInstance = nullptr;
    m_testAppClass = nullptr;
}

std::unique_lock<std::mutex> TestApp::Lock() const
{
    return std::unique_lock<std::mutex>{ m_mutex };
}

JavaVM* TestApp::GetJavaVM()
{
    return m_javaVm;
}

jobject TestApp::GetAppContext()
{
    return m_activityInstance;
}

std::string const& TestApp::GetCurrentPlayerId()
{
    return m_currentPlayerId;
}

jobject TestApp::GetSignInClient()
{
    return m_signInClient;
}

void TestApp::GetBufferFromFile(const char* filename, std::vector<char>& fileBuffer)
{
    JNIEnv* env = JniEnvFromJavaVm(m_javaVm);

    jbyteArray fileArray = (jbyteArray) env->CallObjectMethod(m_activityInstance, m_getBufferFromFileMethod, env->NewStringUTF(filename));
    if (env->ExceptionCheck())
    {
        LOGE("Failed to get buffer from file");
        return;
    }

    jsize fileSize = env->GetArrayLength(fileArray);
    fileBuffer.resize(fileSize);
    env->GetByteArrayRegion(fileArray, 0, fileSize, reinterpret_cast<jbyte*>(&fileBuffer[0]));
}

const char* TestApp::GetDeviceToken()
{
    JNIEnv* env = JniEnvFromJavaVm(m_javaVm);
    jstring result = (jstring) env->CallObjectMethod(m_activityInstance, m_getDeviceTokenMethod);

    return env->GetStringUTFChars(result, 0);
}

const char* TestApp::GetServerAuthToken()
{
    JNIEnv* env = JniEnvFromJavaVm(m_javaVm);
    jstring result = (jstring) env->CallObjectMethod(m_activityInstance, m_getServerAuthTokenMethod);

    return env->GetStringUTFChars(result, 0);
}

}
