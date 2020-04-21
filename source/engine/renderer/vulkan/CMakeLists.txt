

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/pavon/PavonEngine/bin)
add_library(pe_vk vulkan.c)
target_link_libraries(pe_vk vulkan)
add_executable(vktest test.c)
target_link_libraries(vktest pe_vk engine m glfw GL freetype pthread asound)
