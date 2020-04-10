cmake_minimum_required(VERSION 3.10.0)
include(ExternalProject)

project(PavonEngine C)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options (-fdiagnostics-color=always)

add_subdirectory(freetype)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)


find_library(log-lib
log)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


include(/home/pavon/sources/PavonEngineC/Source/Engine/CMakeLists.txt) 

set(main_file main.c input.c)     

message("TOOLCHAIN_ANDROID ${CMAKE_C_COMPILER}")

add_library(maincode
SHARED
        ${main_file}
)

target_link_libraries(maincode
        engine
        EGL
        GLESv2
        android
        native_app_glue        
        ${log-lib}
        OpenSLES
        freetype        
        )
