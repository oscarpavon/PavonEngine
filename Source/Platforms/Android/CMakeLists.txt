cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options (-fdiagnostics-color=always)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)


find_library(log-lib
log)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(main_file main.c input.c)


add_library(maincode
SHARED
        ${main_file}
)

target_link_libraries(maincode
        EGL
        GLESv2
        android
        native_app_glue        
        ${log-lib}
        OpenSLES
        ) 
