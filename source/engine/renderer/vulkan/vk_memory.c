#include <engine/renderer/vulkan/vulkan.h>

void pe_vk_memory_get_requirements(VkBuffer buffer){
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(vk_device,buffer,&requirements);



}

uint32_t pe_vk_memory_find_type(uint32_t type_filter, VkMemoryPropertyFlags flags){
    VkPhysicalDeviceMemoryProperties properties;
    vkGetPhysicalDeviceMemoryProperties(vk_physical_device,&properties);

    for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (properties.memoryTypes[i].propertyFlags & flags) == flags) {
            return i;
        }
    }
}