#include "stdafx.h"
#include "PlatformComponents_Android.h"

namespace PlayFab
{

Result<SharedPtr<PlatformComponents_Android>> PlatformComponents_Android::Initialize(
    JavaVM *jvm,
    jobject applicationContext
)
{
    RETURN_HR_INVALIDARG_IF_NULL(jvm);
    RETURN_HR_INVALIDARG_IF_NULL(applicationContext);

    JNIEnv* jniEnv = nullptr;

    jint result = jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK)
    {
        TRACE_ERROR("Failed to get JNIEnv");
        return E_FAIL;
    }

    jclass localUserClass = jniEnv->FindClass("com/microsoft/playfab/core/LocalUser");
    if (localUserClass == nullptr)
    {
        TRACE_ERROR("Failed to get LocalUser class");
        return E_FAIL;
    }

    jclass globalLocalUserClass = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(localUserClass));

    Allocator<PlatformComponents_Android> a;
    auto platformComponents = SharedPtr<PlatformComponents_Android>(
    new (a.allocate(1)) PlatformComponents_Android
        {
            jvm,
            applicationContext,
            globalLocalUserClass
        }
    );

    return std::move(platformComponents);
}

PlatformComponents_Android::PlatformComponents_Android(
    JavaVM *jvm,
    jobject applicationContext,
    jclass localUserClass
) :
    m_jvm{ jvm },
    m_applicationContext{ applicationContext },
    m_localUserClass{ localUserClass }
{
}

PlatformComponents_Android::~PlatformComponents_Android()
{
    JNIEnv* jniEnv = nullptr;
    bool isThreadAttached = false;
    jint getEnvResult = m_jvm->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6);

    if (getEnvResult == JNI_EDETACHED)
    {
        jint attachThreadResult = m_jvm->AttachCurrentThread(&jniEnv, nullptr);
        if (attachThreadResult == JNI_OK)
        {
            isThreadAttached = true;
        }
    }

    if (jniEnv != nullptr)
    {
        jniEnv->DeleteGlobalRef(m_localUserClass);
    }

    if (isThreadAttached)
    {
        m_jvm->DetachCurrentThread();
    }
}
JavaVM* PlatformComponents_Android::GetJavaVm() const
{
    return m_jvm;
}

jobject PlatformComponents_Android::GetApplicationContext() const
{
    return m_applicationContext;
}

jclass PlatformComponents_Android::GetLocalUserClass() const
{
    return m_localUserClass;
}

}