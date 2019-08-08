cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options (-fdiagnostics-color=always)

add_compile_definitions(PRIVATE EDITOR)

add_subdirectory(Engine/source)
add_subdirectory(Engine/source/Editor)
