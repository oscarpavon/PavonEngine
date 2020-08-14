#include "commands.h"
#include <engine/macros.h>
#include <engine/renderer/vulkan/vulkan.h>
#include <engine/renderer/vulkan/framebuffer.h>

void pe_vk_command_init(){

    VkCommandPoolCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = 0;
    info.queueFamilyIndex = q_graphic_family;

    vkCreateCommandPool(vk_device,&info,NULL,&pe_vk_commands_pool);


    array_init(&pe_vk_command_buffers,sizeof(VkCommandBuffer),pe_vk_framebuffers.count);
    pe_vk_command_buffers.count = pe_vk_framebuffers.count;


    VkCommandBufferAllocateInfo bufferinfo;
    ZERO(bufferinfo);
    bufferinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferinfo.commandPool = pe_vk_commands_pool;
    bufferinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufferinfo.commandBufferCount = pe_vk_command_buffers.count;

    vkAllocateCommandBuffers(vk_device,&bufferinfo,pe_vk_command_buffers.data);


    for(int i = 0; i < pe_vk_command_buffers.count ; i++){
        VkCommandBufferBeginInfo begininfo;
        ZERO(begininfo);
        begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begininfo.flags = 0;
        begininfo.pInheritanceInfo = NULL;

        VkCommandBuffer* buffer = array_get(&pe_vk_command_buffers,i);
        vkBeginCommandBuffer(*(buffer),&begininfo);
    }
}