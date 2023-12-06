
#ifndef VK_UNIFORM
#define VK_UNIFORM

#include "vulkan.h"

#include "vk_buffer.h"

#include <engine/model.h>

PEVKBufferCreateInfo buffer_color;

void pe_vk_uniform_buffer_update(uint32_t image_index);

void pe_vk_create_uniform_buffers(PModel* model);

#endif
