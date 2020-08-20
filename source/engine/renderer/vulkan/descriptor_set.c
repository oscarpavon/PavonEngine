#include "descriptor_set.h"

#include <engine/engine.h>
#include "swap_chain.h"

void pe_vk_create_descriptor_set_layout(){
    VkDescriptorSetLayoutBinding binding;
    ZERO(binding);
    binding.binding = 0;
    binding.descriptorCount = 1;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


    VkDescriptorSetLayoutCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = 1;
    info.pBindings = &binding;

    vkCreateDescriptorSetLayout(vk_device,&info,NULL,&pe_vk_descriptor_set_layout);

}

