
#include "vulkan.h"
#include <engine/macros.h>
#include <engine/log.h>
#include "vk_memory.h"
#include "vk_buffer.h"



void pe_vk_image_create(uint32_t width, 
        uint32_t height,void* pixels,  VkImage* textureImage){



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
VkDeviceSize image_size = width * height * 4;

VkMemoryRequirements image_memory_requirements;
vkGetImageMemoryRequirements(vk_device,*(textureImage),&image_memory_requirements);

PEVKBufferCreateInfo buffer_info;
buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
buffer_info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

pe_vk_buffer_create(&buffer_info);

void* data;
vkMapMemory(vk_device,buffer_info.buffer_memory,0,4,0,&data);
memcpy(data,pixels,image_size);
vkUnmapMemory(vk_device,buffer_info.buffer_memory);


    VkMemoryAllocateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.allocationSize = image_memory_requirements.size;
    info.memoryTypeIndex = pe_vk_memory_find_type(image_memory_requirements.memoryTypeBits,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkDeviceMemory memory;
    vkAllocateMemory(vk_device,&info,NULL,&memory);

    vkBindImageMemory(vk_device,*(textureImage),memory,0);

}
