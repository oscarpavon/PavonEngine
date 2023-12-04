
#include "engine/images.h"
#include "vulkan.h"
#include <engine/macros.h>
#include <engine/log.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>
#include <wchar.h>
#include "vk_memory.h"
#include "vk_buffer.h"
#include "commands.h"
#include "vk_images.h"



void pe_vk_transition_image_layout(VkImage image, VkFormat format,
                                   VkImageLayout old_layout,
                                   VkImageLayout new_layout) {
  VkCommandBuffer command = pe_vk_begin_single_time_cmd();

  VkImageMemoryBarrier barrier = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .oldLayout = old_layout,
      .newLayout = new_layout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = image,
      .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .subresourceRange.baseMipLevel = 0,
      .subresourceRange.levelCount = 1,
      .subresourceRange.baseArrayLayer = 0,
      .subresourceRange.layerCount = 1
  };

  vkCmdPipelineBarrier(command, 0 /* TODO */, 0 /* TODO */, 0, 0, NULL, 0, NULL,
                       1, &barrier);

  pe_vk_end_single_time_cmd(command);
}

void pe_vk_image_copy_buffer(VkBuffer buffer, VkImage image, uint32_t width,
                             uint32_t height) {
  VkCommandBuffer command = pe_vk_begin_single_time_cmd();

  VkBufferImageCopy region = {.bufferOffset = 0,
                              .bufferRowLength = 0,
                              .bufferImageHeight = 0,
                              .imageSubresource.aspectMask =
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                              .imageSubresource.mipLevel = 0,
                              .imageSubresource.baseArrayLayer = 0,
                              .imageSubresource.layerCount = 0,
                              .imageOffset = {0, 0, 0},
                              .imageExtent = {width, height, 1}};

  vkCmdCopyBufferToImage(command, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  pe_vk_end_single_time_cmd(command);
}

void pe_vk_image_create(uint32_t width, uint32_t height, void *pixels,
                        VkImage *texture_image) {

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
  imageInfo.usage =
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0; // Optional

  if (vkCreateImage(vk_device, &imageInfo, NULL, texture_image) != VK_SUCCESS) {
    LOG("failed to create image!");
  }


  VkMemoryRequirements image_memory_requirements;
  vkGetImageMemoryRequirements(vk_device, *(texture_image),
                               &image_memory_requirements);


  VkMemoryAllocateInfo info;
  ZERO(info);
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.allocationSize = image_memory_requirements.size;
  info.memoryTypeIndex =
      pe_vk_memory_find_type(image_memory_requirements.memoryTypeBits,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  vkAllocateMemory(vk_device, &info, NULL, &pe_vk_texture_image_memory);

  vkBindImageMemory(vk_device, *(texture_image), pe_vk_texture_image_memory, 0);
}

void pe_vk_create_texture_image(){
  PTexture texture;
  ZERO(texture);
  pe_load_texture("/sdcard/Download/chess/texture.jpg", &texture);

  
  VkDeviceSize image_size = texture.image.width * texture.image.heigth * 4;

  PEVKBufferCreateInfo buffer_info;
  ZERO(buffer_info);
  buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  buffer_info.size = image_size;
  pe_vk_buffer_create(&buffer_info);
  //LOG("Image size %li\n", image_size);
  if(texture.image.pixels_data == NULL){
    return;
  }
  void *data;
  vkMapMemory(vk_device, buffer_info.buffer_memory, 0, image_size, 0, &data);
  // memcpy(data, texture.image.pixels_data, image_size);
  // vkUnmapMemory(vk_device, buffer_info.buffer_memory);
}
