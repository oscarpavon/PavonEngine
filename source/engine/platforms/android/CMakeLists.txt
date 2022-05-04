cmake_minimum_required(VERSION 3.6.0)
project(PavonEngineAndroid)
set(CMAKE_C_STANDARD_REQUIRED ON)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r23b)

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)


# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")



add_compile_definitions(PUBLIC ANDROID)


include_directories(/data/data/com.termux/files/home/PavonEngine/source/)

include_directories(
    ${ANDROID_NDK}/sources/android/native_app_glue)

set(link 
    android
    native_app_glue
    EGL
    log
    GLESv2
    pavon_engine
    m
    #${FREETYPE_LIBRARIES}
    )


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/arm64-v8a)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/arm64-v8a)

add_library(native-activity SHARED main.c )
    



target_include_directories(native-activity PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_directories(native-activity PUBLIC /data/data/com.termux/files/home/PavonEngine/lib)
target_link_libraries(native-activity
    ${link})

