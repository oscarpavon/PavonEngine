 
include(FindFreetype)
include_directories(${FREETYPE_INCLUDE_DIRS} ./source)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(/usr/local/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/pavon/PavonEngine/bin)
add_library(pe_vk vulkan.c debug.c swap_chain.c)

target_link_libraries(pe_vk vulkan)

add_executable(vktest test.c)
target_link_libraries(vktest pe_vk engine m glfw 
	${FREETYPE_LIBRARIES}
	${OPENGL_LIBRARIES}
	pthread)
