#include "TestAppPch.h"
#include "AndroidTestApp.h"
#include "PlatformUtilsAndroid.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "RapidJson.h"
#include "TestRunner.h"
#include "../PlatformUtils.h"

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
    jobject context
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
    PlayFab::Test::TestRunner testRunner;
    THROW_IF_FAILED(testRunner.Initialize());

    while (!testRunner.Update())
    {
        PlayFab::Test::Platform::Sleep(10);
    }

    bool allTestsPassed = testRunner.Cleanup();

    return allTestsPassed ? true : false;
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

}
