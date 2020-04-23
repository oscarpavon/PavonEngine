#ifndef VULKAN
#define VULKAN

#include <vulkan/vulkan.h>

int pe_vk_init();
void pe_vk_end();

VkInstance vk_instance;
VkPhysicalDevice vk_physical_device;
VkDevice vk_device;
VkQueue vk_queue;
VkQueue vk_present_queue;
VkSurfaceKHR vk_surface;

uint32_t q_graphic_family;
uint32_t q_present_family;

#endif
