
#include "sync.h"
#include <engine/macros.h>

void pe_vk_semaphores_create(){

    VkSemaphoreCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    vkCreateSemaphore(vk_device,&info, NULL, &pe_vk_semaphore_images_available);
    vkCreateSemaphore(vk_device,&info, NULL, &pe_vk_semaphore_render_finished);

}