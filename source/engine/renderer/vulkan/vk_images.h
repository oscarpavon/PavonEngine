#include <engine/renderer/vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

void pe_vk_image_create(uint32_t width, uint32_t height, void *pixels,
                        VkImage *textureImage);

VkImage pe_vk_texture_image;
VkDeviceMemory pe_vk_texture_image_memory;
void pe_vk_create_texture_image();
