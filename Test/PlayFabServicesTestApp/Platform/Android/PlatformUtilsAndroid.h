#include <android/log.h>
#include <jni.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidTestApp", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidTestApp", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "AndroidTestApp", __VA_ARGS__))

JNIEnv* JniEnvFromJavaVm(JavaVM* javaVm);
JavaVM* JavaVmFromJniEnv(JNIEnv* env);