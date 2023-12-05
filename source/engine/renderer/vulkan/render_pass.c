
#include <engine/renderer/vulkan/vulkan.h>
#include <engine/macros.h>
#include <vulkan/vulkan_core.h>
#include "framebuffer.h"
#include "commands.h"
#include "draw.h"
#include"pipeline.h" 
#include <engine/log.h>
#include <engine/engine.h>
#include "swap_chain.h"
void pe_vk_create_render_pass(){

    VkAttachmentDescription color_attachment;
    ZERO(color_attachment);
    color_attachment.format = pe_vk_swch_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkFormat depth_format = VK_FORMAT_D32_SFLOAT;
    VkAttachmentDescription depth_attachment = {};
    depth_attachment.format = depth_format; 
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_ref;
    ZERO(color_ref);
    color_ref.attachment = 0;
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkSubpassDescription subpass;
    ZERO(subpass);
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_ref;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency;
    ZERO(dependency);
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;

    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkAttachmentDescription attachments[2];
    ZERO(attachments);
    attachments[0]  = color_attachment;
    attachments[1] = depth_attachment;

    VkRenderPassCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 2;
    info.pAttachments = attachments;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;
    VKVALID(vkCreateRenderPass(vk_device, &info, NULL, &pe_vk_render_pass),
            "Can't create render pass");
}

void pe_vk_start_render_pass(int i){

  VkFramebuffer *framebuffer = array_get(&pe_vk_framebuffers, i);
  VkOffset2D offset = {0, 0};
  VkClearValue clear_color = {0.5f, 0.0f, 1.0f, 1.0f};
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

  pe_vk_draw_commands(cmd_buffer, i);

  vkCmdEndRenderPass(*(cmd_buffer));


  pe_vk_commands_end(i);
}
