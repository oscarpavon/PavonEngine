
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/pavon/PavonEngine/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

add_library(pe_vk vulkan.c debug.c swap_chain.c 
			shader_module.c pipeline.c 
			render_pass.c framebuffer.c
			commands.c images_view.c draw.c
			sync.c vk_vertex.c vk_memory.c
			descriptor_set.c uniform_buffer.c
			vk_buffer.c vk_descriptor_pool.c
            vk_images.c
			)

target_link_libraries(pe_vk vulkan)

