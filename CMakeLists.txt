cmake_minimum_required(VERSION 3.6.0)
project(PavonEngine)
set(CMAKE_BUILD_TYPE=Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wno-switch -Wno-return-type -fcommon -Wno-incompatible-pointer-types -I/usr/X11R6/include -I/usr/local/include -I./source")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g ")

if(ANDROID)
        set(ANDROID_NDK /data/data/com.termux/files/home/android-ndk-r23b)
        include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
#include_directories(/home/pavon/sources/freetype2/include)

endif()

add_compile_options (-fdiagnostics-color=always)

add_compile_definitions(PUBLIC EDITOR)

#add_compile_definitions(PUBLIC LINUX)
add_compile_definitions(PUBLIC OPENGL_ES2)
#add_compile_definitions(PUBLIC BSD)

add_compile_definitions(PUBLIC ANDROID)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS} ./source)
add_subdirectory(source/engine)
add_subdirectory(source/editor)
#add_subdirectory(source/engine/renderer/vulkan)
#add_subdirectory(source/editor/utils/pcmd)
#C:add_subdirectory(demos/)
