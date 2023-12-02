#include "images_view.h"
#include <engine/macros.h>
#include <string.h>
#include <engine/renderer/vulkan/swap_chain.h>
#include <engine/log.h>
#include <engine/macros.h>

#define PE_IMAGES_VIEWS_COUNT 4

void pe_vk_create_images_views() {
  ZERO(pe_vk_images_views);
  array_init(&pe_vk_images_views, sizeof(VkImageView), PE_IMAGES_VIEWS_COUNT);

  // images view count equal to pe_vk_images array
  for (size_t i = 0; i < PE_IMAGES_VIEWS_COUNT; i++) {
    VkImageViewCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

    info.image = pe_vk_images[i];
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = pe_vk_swch_format;

    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;

    VkImageView image_view;
    ZERO(image_view);
    vkCreateImageView(vk_device, &info, NULL, &image_view);
    array_add(&pe_vk_images_views, &image_view);
  }
}
