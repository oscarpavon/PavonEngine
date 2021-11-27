
#include "vulkan.h"
#include <engine/macros.h>
#include <engine/log.h>
void pe_vk_image_create(uint32_t width, uint32_t height, VkImage* textureImage){



VkImageCreateInfo imageInfo;
ZERO(imageInfo);

imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
imageInfo.imageType = VK_IMAGE_TYPE_2D;
imageInfo.extent.width = width;
imageInfo.extent.height = height;
imageInfo.extent.depth = 1;
imageInfo.mipLevels = 1;
imageInfo.arrayLayers = 1;
imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;

imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
imageInfo.flags = 0; // Optional

if (vkCreateImage(vk_device, &imageInfo, NULL , textureImage) != VK_SUCCESS) {
    LOG("failed to create image!");
}



}
