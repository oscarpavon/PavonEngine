#include <engine/renderer/vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


VkImage pe_vk_texture_image;
VkDeviceMemory pe_vk_texture_image_memory;
void pe_vk_create_texture_image();
void pe_vk_image_create(uint32_t width, uint32_t height, VkImage* texture_image);