#include "draw.h"
#include "commands.h"
#include "pipeline.h"
#include "swap_chain.h"
#include "sync.h"
#include <engine/macros.h>
#include "vk_vertex.h"
#include <engine/engine.h>

void pe_vk_uniform_buffer_update(uint32_t image_index){
  
    PEUniformBufferObject ubo;
    ZERO(ubo);

    glm_mat4_identity(ubo.view);
    glm_mat4_identity(ubo.projection);
    glm_mat4_identity(ubo.model);

    vec3 up;
    vec3 front;
    vec3 position;

    init_vec3(1.0f, 0 ,  0.0f, up);
    init_vec3(0.0f, 0.0f,  1.0f, front);
    init_vec3(0,0,0, position);

    vec3 look_pos;
    glm_vec3_add(position, front, look_pos);

    glm_lookat(position, look_pos, front , ubo.view);

    glm_perspective(45.f, camera_width_screen / camera_heigth_screen, 0.001f,
                    5000.f, ubo.projection);

    
    void* data;
  
  
}


void pe_vk_draw(int i){
    VkCommandBuffer* cmd_buffer = array_get(&pe_vk_command_buffers,i);


    vkCmdBindPipeline(*(cmd_buffer),VK_PIPELINE_BIND_POINT_GRAPHICS,pe_vk_pipeline);

    VkBuffer vertex_buffers[] = {vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(*(cmd_buffer),0,1,vertex_buffers,offsets);

    vkCmdBindIndexBuffer(*(cmd_buffer),index_buffer,0,VK_INDEX_TYPE_UINT16);

    //vkCmdDraw(*(cmd_buffer), vertices.count, 1, 0, 0);
    
    vkCmdDrawIndexed(*(cmd_buffer),model_indices.count,1,0,0,0);


}

void pe_vk_draw_frame(){

    uint32_t image_index;

    vkAcquireNextImageKHR(vk_device,pe_vk_swap_chain,UINT64_MAX,pe_vk_semaphore_images_available, VK_NULL_HANDLE,&image_index);

    pe_vk_uniform_buffer_update(image_index);
    
    VkSubmitInfo submit_info;
    ZERO(submit_info);
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = {pe_vk_semaphore_images_available};
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    submit_info.commandBufferCount = 1;
    VkCommandBuffer* cmd_buffer = array_get(&pe_vk_command_buffers,image_index);
    submit_info.pCommandBuffers = cmd_buffer;

    VkSemaphore singal_semaphore[] = {pe_vk_semaphore_render_finished};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = singal_semaphore;

    vkQueueSubmit(vk_queue,1,&submit_info,VK_NULL_HANDLE);


    VkPresentInfoKHR present_info;
    ZERO(present_info);
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = singal_semaphore;


    VkSwapchainKHR swap_chains[] = {pe_vk_swap_chain};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;

    vkQueuePresentKHR(vk_queue,&present_info);

    vkQueueWaitIdle(vk_queue);

}


