#include <jni.h>

/// <summary>
/// The JNI Methods for interacting with our C++ Singleton
/// </summary>
extern "C"
{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

JNIEXPORT void JNICALL Java_com_microsoft_playfab_sdk_AndroidTestClient_InitializeApp(
    JNIEnv* env,
    jobject instance,
    jobject context
);

void Java_com_microsoft_playfab_sdk_AndroidTestClient_UpdateInstances(
    JNIEnv* env,
    jobject instance,
    jobject context
);

bool Java_com_microsoft_playfab_sdk_AndroidTestClient_RunTests();

}
