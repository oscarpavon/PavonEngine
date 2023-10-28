
#include <engine/renderer/vulkan/vulkan.h>
#include <engine/macros.h>
#include "framebuffer.h"
#include "commands.h"
#include "draw.h"
void pe_vk_create_render_pass(){

    VkAttachmentDescription color;
    ZERO(color);
    color.format = pe_vk_swch_format;
    color.samples = VK_SAMPLE_COUNT_1_BIT;
    color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference color_ref;
    ZERO(color_ref);
    color_ref.attachment = 0;
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkSubpassDescription subpass;
    ZERO(subpass);
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_ref;


    VkSubpassDependency dependency;
    ZERO(dependency);
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;

    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;



    VkRenderPassCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &color;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    vkCreateRenderPass(vk_device,&info,NULL,&pe_vk_render_pass);

}

void pe_vk_start_render_pass(int i){

  VkFramebuffer *framebuffer = array_get(&pe_vk_framebuffers, i);
  VkOffset2D offset = {0, 0};
  VkClearValue clear_color = {0.0f, 0.0f, 1.0f, 1.0f};
  VkCommandBuffer *cmd_buffer = array_get(&pe_vk_command_buffers, i);

  VkRenderPassBeginInfo info;
  ZERO(info);
  info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  info.renderPass = pe_vk_render_pass;
  info.framebuffer = *(framebuffer);
  info.renderArea.offset = offset;
  info.renderArea.extent = pe_vk_swch_extent;
  info.clearValueCount = 1;
  info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(*(cmd_buffer), &info, VK_SUBPASS_CONTENTS_INLINE);

  pe_vk_draw(i);
//  vkCmdDraw(*(cmd_buffer), 3,1,0,0);

  vkCmdEndRenderPass(*(cmd_buffer));

  pe_vk_commands_end(i);
}
