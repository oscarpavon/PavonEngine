#include <engine/renderer/vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


VkImage pe_vk_texture_image;
VkImageView pe_vk_texture_image_view;
VkSampler pe_vk_texture_sampler;

VkImage pe_vk_depth_image;
VkDeviceMemory pe_vk_depth_image_memory;
VkImageView pe_vk_depth_image_view;

VkDeviceMemory pe_vk_texture_image_memory;
void pe_vk_create_texture_image();
void pe_vk_create_image(uint32_t width, uint32_t height, VkFormat format,
                        VkImageTiling tiling, VkImageUsageFlags usage,
                        VkMemoryPropertyFlags properties,
                        VkImage *texture_image, VkDeviceMemory *image_memory);
void pe_vk_create_depth_resources();
void pe_vk_create_texture_sampler();
