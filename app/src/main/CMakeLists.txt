cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)


find_library(log-lib
        log)


set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

include_directories(/usr/include/cglm)

add_library(maincode
    SHARED
        main.c
        model.c
        vector.c
        file_loader.c
        level.c
        camera.c
        images.c
        input.c
        audio.c
        gui.c
    )

target_link_libraries(maincode
        android
        native_app_glue
        ${log-lib}
        EGL
        GLESv2
        OpenSLES
        )