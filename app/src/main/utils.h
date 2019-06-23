#ifndef UTILS_H
#define UTILS_H

#include <android_native_app_glue.h>
#include <android/log.h>

#include <cglm.h>

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))


static inline void init_vec3(float x, float y, float z , vec3 vec){
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

#endif