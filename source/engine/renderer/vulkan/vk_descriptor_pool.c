#include "vk_descriptor_pool.h"


void pe_vk_descriptor_pool_create(){
    VkDescriptorPoolSize pool_size;
    ZERO(pool_size);
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = 4;

    VkDescriptorPoolCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.poolSizeCount = 1;
    info.pPoolSizes = &pool_size;
    info.maxSets = 4;

    
    vkCreateDescriptorPool(vk_device,&info,NULL,&pe_vk_descriptor_pool);




}