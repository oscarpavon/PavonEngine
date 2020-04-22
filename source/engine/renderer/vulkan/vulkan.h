#ifndef VULKAN
#define VULKAN

#include <vulkan/vulkan.h>

int pe_vk_init();
void pe_vk_end();

VkInstance vk_instance;
VkPhysicalDevice vk_physical_device;
VkDevice vk_device;
uint32_t q_graphic_family;


#endif
