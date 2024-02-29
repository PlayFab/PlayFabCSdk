#pragma once

#include <jni.h>
#include <mutex>
#include <string>
#include <thread>

namespace AndroidTestApp
{

enum class ThreadState
{
    None,
    Background,
    Foreground,
};

class TestApp
{
public:
    static TestApp& GetInstance();

    static void DoWork(TestApp* testApp);

    void AppInitialize(
        JNIEnv* env,
        jobject activityInstance,
        jobject context
    );

    void UpdateInstances(
        JNIEnv* env,
        jobject activityInstance,
        jobject context
    );

    TestApp(TestApp const&) = delete;
    void operator=(TestApp const&) = delete;

    bool RunTests();
    JavaVM* GetJavaVM();
    jobject GetAppContext();
    void GetBufferFromFile(const char* filename, std::vector<char>& fileBuffer);
    const char* GetDeviceToken();

private: // Private constructor for singleton behavior
    TestApp();

private:
    std::unique_lock<std::mutex> Lock() const;
    mutable std::mutex m_mutex;
    bool m_initialized;
    JavaVM* m_javaVm;
    jobject m_context;
    jobject m_activityInstance;
    jclass m_testAppClass;

    jmethodID m_getBufferFromFileMethod;
    jmethodID m_getDeviceTokenMethod;
};

}