
#include "sync.h"
#include "vulkan.h"
#include <engine/macros.h>
#include <vulkan/vulkan_core.h>

void pe_vk_semaphores_create(){

  VkSemaphoreCreateInfo info;
  ZERO(info);
  info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_info;
  ZERO(fence_info);
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;


  vkCreateSemaphore(vk_device, &info, NULL, &pe_vk_semaphore_images_available);
  vkCreateSemaphore(vk_device, &info, NULL, &pe_vk_semaphore_render_finished);
  vkCreateFence(vk_device,&fence_info,NULL,&pe_vk_fence_in_flight);
}
