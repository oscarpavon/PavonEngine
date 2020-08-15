#include <engine/renderer/vulkan/vulkan.h>


VkSemaphore pe_vk_semaphore_images_available;
VkSemaphore pe_vk_semaphore_render_finished;

void pe_vk_semaphores_create();