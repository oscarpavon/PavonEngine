#include "vulkan.h"
#include <engine/macros.h>
#include <engine/array.h>


VkPipelineLayout pe_vk_pipeline_layout;

VkDescriptorSetLayout pe_vk_descriptor_set_layout;
Array pe_vk_descriptor_sets;

void pe_vk_create_descriptor_set_layout();
