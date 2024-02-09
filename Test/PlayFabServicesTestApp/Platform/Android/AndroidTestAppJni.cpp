#include "TestAppPch.h"
#include "AndroidTestAppJni.h"
#include <httpClient/pal.h>
#include <httpClient/trace.h>
#include "AndroidTestApp.h"

extern "C"
{

jint JNI_OnLoad(JavaVM*, void*)
{
    return JNI_VERSION_1_6;
}

void Java_com_microsoft_playfab_sdk_AndroidTestClient_InitializeApp(
    JNIEnv* env,
    jobject instance,
    jobject context
)
{
    AndroidTestApp::TestApp::GetInstance().AppInitialize(
        env,
        instance,
        context
    );
}

void Java_com_microsoft_playfab_sdk_AndroidTestClient_UpdateInstances(
    JNIEnv* env,
    jobject instance,
    jobject context
)
{
    AndroidTestApp::TestApp::GetInstance().UpdateInstances(
        env,
        instance,
        context
    );
}

bool Java_com_microsoft_playfab_sdk_AndroidTestClient_RunTests()
{
    return AndroidTestApp::TestApp::GetInstance().RunTests();
}

}
