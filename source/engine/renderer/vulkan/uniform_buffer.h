
#ifndef VK_UNIFORM
#define VK_UNIFORM

#include "vulkan.h"

#include "vk_buffer.h"

Array pe_vk_uniform_buffers;
Array pe_vk_uniform_buffers_memory;

PEVKBufferCreateInfo buffer_color;

void pe_vk_uniform_buffer_update(uint32_t image_index);

void pe_vk_uniform_buffer_create();

#endif
