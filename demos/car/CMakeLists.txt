
cmake_minimum_required(VERSION 3.6.0)
project(Car)
include(../../include.cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wno-switch -Wno-return-type -fcommon -Wno-incompatible-pointer-types -I/usr/X11R6/include -I/usr/local/include -I/home/pavon/PavonEngine/source -I/home/pavon/PavonEngine/source/engine")
link_directories(../../lib)
add_executable(car car.c)

target_link_libraries(car engine ${engine_link})
