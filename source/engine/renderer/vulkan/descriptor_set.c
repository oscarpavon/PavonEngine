#include "descriptor_set.h"

#include <engine/engine.h>
#include "swap_chain.h"
#include "vk_descriptor_pool.h"
#include <engine/array.h>

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

void pe_vk_descriptor_set_create(){

   VkDescriptorSetLayout layouts[] = {pe_vk_descriptor_set_layout,pe_vk_descriptor_set_layout,
   pe_vk_descriptor_set_layout,pe_vk_descriptor_set_layout};

    array_init(&pe_vk_descriptor_sets,sizeof(VkDescriptorSetLayout),4);
    
    array_resize(&pe_vk_descriptor_sets,4);

    VkDescriptorSetAllocateInfo alloc_info;
    ZERO(alloc_info);
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pe_vk_descriptor_pool;
    alloc_info.descriptorSetCount = 4;//swap chain images count
    alloc_info.pSetLayouts = layouts;

    vkAllocateDescriptorSets(vk_device,&alloc_info,pe_vk_descriptor_sets.data);


}
