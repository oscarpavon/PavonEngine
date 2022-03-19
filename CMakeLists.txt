cmake_minimum_required(VERSION 3.6.0)
project(PavonEngine)
set(CMAKE_BUILD_TYPE=Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC -Wno-switch -Wno-return-type -fcommon -Wno-incompatible-pointer-types -I/usr/X11R6/include -I/usr/local/include -I./source")

add_compile_options (-fdiagnostics-color=always)

#add_compile_definitions(PUBLIC EDITOR)

#add_compile_definitions(PUBLIC LINUX)
add_compile_definitions(PUBLIC OPENGL_ES2)
#add_compile_definitions(PUBLIC BSD)

#add_compile_definitions(PUBLIC ANDROID)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS} ./source)
add_subdirectory(source/engine)
#add_subdirectory(source/editor)
#add_subdirectory(source/engine/renderer/vulkan)
#add_subdirectory(source/editor/utils/pcmd)
#add_subdirectory(demos/)
