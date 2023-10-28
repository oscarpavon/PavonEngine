#include "vulkan.h"
#include <engine/macros.h>
#include <engine/array.h>


VkPipelineLayout pe_vk_pipeline_layout;

VkDescriptorSetLayout pe_vk_descriptor_set_layout;
Array pe_vk_descriptor_sets;

VkDescriptorPool pe_vk_descriptor_pool;

void pe_vk_descriptor_pool_create();
void pe_vk_descriptor_set_create();

void pe_vk_create_descriptor_set_layout();
