cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options (-fdiagnostics-color=always)

add_compile_definitions(PUBLIC EDITOR)

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})
add_subdirectory(Source/Engine)
add_subdirectory(Source/Editor)
