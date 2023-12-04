#ifndef VK_BUFFER
#define VK_BUFFER
#include "vulkan.h"

typedef struct PEVKBufferCreateInfo{
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags properties;
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
}PEVKBufferCreateInfo;


void pe_vk_buffer_create(PEVKBufferCreateInfo* buffer_info);
void pe_vk_copy_buffer(VkBuffer source, VkBuffer destination, VkDeviceSize size);

#endif
