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

set(CMAKE_ANDROID_ARCH_ABI javon)
message("My Android Arch ${CMAKE_ANDROID_ARCH_ABI}")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")



add_compile_definitions(PUBLIC ANDROID)
add_compile_definitions(PUBLIC DEBUG)


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
    peditor
    #${FREETYPE_LIBRARIES}
    freetype
    
    )

#set(ANDROID_ABI arm64-v8a)
#set(ANDROID_ABI armabi-v7a)
set(ANDROID_ABI armabi)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/${ANDROID_ABI})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/${ANDROID_ABI})

add_library(pavon_engine_android SHARED main.c)
    



target_include_directories(pavon_engine_android PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_directories(pavon_engine_android PUBLIC /data/data/com.termux/files/home/PavonEngine/lib)
target_link_libraries(pavon_engine_android
    ${link})

