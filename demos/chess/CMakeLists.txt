
cmake_minimum_required(VERSION 3.6.0)
project(Chess)
include(../../include.cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wno-switch -Wno-return-type -fcommon -Wno-incompatible-pointer-types -I/usr/X11R6/include -I/usr/local/include -I../../../../source")
link_directories(../../../lib)
add_executable(chess chess.c)

target_link_libraries(chess pavon_engine ${engine_link})
