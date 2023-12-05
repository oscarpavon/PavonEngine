#include "vulkan.h"
#include <engine/macros.h>
#include <engine/array.h>


VkPipelineLayout pe_vk_pipeline_layout;
VkPipelineLayout pe_vk_pipeline_layout_with_descriptors;

VkDescriptorSetLayout pe_vk_descriptor_set_layout;
VkDescriptorSetLayout pe_vk_descriptor_set_layout_with_texture;

void pe_vk_descriptor_pool_create(PModel* model);
void pe_vk_create_descriptor_sets(PModel* model);

void pe_vk_create_descriptor_set_layout();
void pe_vk_create_descriptor_set_layout_with_texture();
