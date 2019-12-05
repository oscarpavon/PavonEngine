cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_BUILD_TYPE=Debug)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wno-incompatible-pointer-types")
add_compile_options (-fdiagnostics-color=always)

add_compile_definitions(PUBLIC EDITOR)

if(VULKAN)
	message("Vulkan Version")
	add_compile_definitions(PUBLIC VULKAN)
endif()
if(OPENGL)
	message("OpenGL Version")
	add_compile_definitions(PUBLIC OPENGL)
endif()

include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS})
add_subdirectory(Source/Engine)
add_subdirectory(Source/Editor)
