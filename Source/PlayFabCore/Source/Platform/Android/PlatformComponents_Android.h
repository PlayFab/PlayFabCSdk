#pragma once

#include "jni.h"

namespace PlayFab
{

class PlatformComponents_Android
{
public:
    static Result<SharedPtr<PlatformComponents_Android>> Initialize(JavaVM *jvm, jobject applicationContext);

    ~PlatformComponents_Android();

    JavaVM *GetJavaVm() const;

    jobject GetApplicationContext() const;

    jclass GetLocalUserClass() const;

private:
    PlatformComponents_Android(
        JavaVM *jvm,
        jobject applicationContext,
        jclass localUserClass
    );

    JavaVM *m_jvm;
    jobject m_applicationContext;
    jclass m_localUserClass;
};

}