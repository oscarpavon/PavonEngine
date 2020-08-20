#include "vulkan.h"

typedef struct PEVKBufferCreateInfo{
    VkDeviceSize size;
    VkBufferUsageFlags flags;
    VkMemoryPropertyFlags properties;
    VkBuffer* buffer;
    VkDeviceMemory* buffer_memory;
}PEVKBufferCreateInfo;