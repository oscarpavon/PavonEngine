
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(source 
	pcmd.c
	../../../engine/platforms/linux/network.c
	)
add_executable(pcmd ${source})
