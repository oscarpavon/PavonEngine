#include "vulkan.h"
#include <engine/macros.h>
#include <engine/array.h>

Array pe_vk_uniform_buffers;
Array pe_vk_uniform_buffers_memory;

VkDescriptorSetLayout pe_vk_descriptor_set_layout;
VkPipelineLayout pe_vk_pipeline_layout;

void pe_vk_create_descriptor_set_layout();
