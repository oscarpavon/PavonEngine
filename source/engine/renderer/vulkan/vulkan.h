#ifndef PEVULKAN_H
#define PEVULKAN_H

#define VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan.h>
#include <engine/array.h>
#include <engine/macros.h>
#include "draw.h"

#define VKVALID(f,message) if(f != VK_SUCCESS){LOG("%s \n",message);}

int pe_vk_init();
void pe_vk_end();

VkInstance vk_instance;
VkPhysicalDevice vk_physical_device;
VkDevice vk_device;
VkQueue vk_queue;

VkSurfaceKHR vk_surface;


VkRenderPass pe_vk_render_pass;


uint32_t q_graphic_family;
uint32_t q_present_family;


bool pe_vk_validation_layer_enable;

bool pe_vk_initialized;
#endif
