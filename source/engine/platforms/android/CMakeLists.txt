cmake_minimum_required(VERSION 3.6.0)
project(PavonEngineAndroid)
#set(CMAKE_C_STANDARD_REQUIRED ON)

# build native_app_glue as a static lib
#set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r23b)

add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)



#add_library(game STATIC game.c)
#include(/data/data/com.termux/files/home/PavonEngine/make_include.txt)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native-activity SHARED main.c)
add_library(tengine STATIC tengine.c)


add_compile_definitions(PUBLIC ANDROID)
#add_executable(sl main.c)

target_include_directories(native-activity PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue)

include_directories(/data/data/com.termux/files/home/PavonEngine/source/)

include_directories(
    ${ANDROID_NDK}/sources/android/native_app_glue)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /data/data/com.termux/files/home/PavonEngine/source/engine/platforms/android/)
#set(CMAKE_BINARY_DIR /data/data/com.termux/files/home/PavonEngine/source/engine/platforms/android/)

#add_subdirectory(/data/data/com.termux/files/home/PavonEngine PavonEngine)

#add_executable(an main.c)


set(link 
    android
    native_app_glue
    EGL
    log
    GLESv2
    tengine
    #pe_android
    pavon_engine
    m
    #${FREETYPE_LIBRARIES}
    )

target_link_directories(tengine PUBLIC /data/data/com.termux/files/home/PavonEngine/lib)
target_link_directories(native-activity PUBLIC /data/data/com.termux/files/home/PavonEngine/lib)

target_link_libraries(tengine pavon_engine)
#target_link_libraries(an ${link})

target_link_libraries(native-activity
    ${link})
