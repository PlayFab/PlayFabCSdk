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
        jobject context,
        jstring currentPlayerId,
        jobject signInClient
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
    std::string const& GetCurrentPlayerId();
    jobject GetSignInClient();
    void GetBufferFromFile(const char* filename, std::vector<char>& fileBuffer);
    const char* GetDeviceToken();
    const char* GetServerAuthToken();

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
    std::string m_currentPlayerId;
    jobject m_signInClient;

    jmethodID m_getBufferFromFileMethod;
    jmethodID m_getDeviceTokenMethod;
    jmethodID m_getServerAuthTokenMethod;
};

}