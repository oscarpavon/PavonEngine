#ifndef ENGINE_LOG
#define ENGINE_LOG

#ifdef ANDROID
#include <android_native_app_glue.h>
#include <android/log.h>

#define LOG(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#else
#include <stdio.h>
#define LOG printf
#endif//DEF ANDROID


#endif // !ENGINE_LOG