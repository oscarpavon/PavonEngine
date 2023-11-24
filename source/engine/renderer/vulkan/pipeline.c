#include "pipeline.h"
#include "descriptor_set.h"
#include "shader_module.h"
#include "uniform_buffer.h"
#include "vk_vertex.h"
#include "render_pass.h"
#include <engine/macros.h>
#include <engine/renderer/vulkan/vulkan.h>
#include <engine/log.h>

VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                  VK_DYNAMIC_STATE_SCISSOR};
typedef struct PPipelineInfo {
  VkPipelineVertexInputStateCreateInfo vertex_input_state;
  VkPipelineViewportStateCreateInfo viewport_state;
  VkPipelineDynamicStateCreateInfo dynamic_state;
  VkPipelineRasterizationStateCreateInfo rasterization_state;
  VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
  VkPipelineMultisampleStateCreateInfo multisample_state;
  VkPipelineColorBlendAttachmentState color_attachment;
  VkPipelineColorBlendStateCreateInfo color_blend_state;
} PPipelineInfo;

PPipelineInfo pe_vk_main_pipeline_info;

VkPipelineDynamicStateCreateInfo pe_vk_pipeline_get_default_dynamic_state(){

  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  ZERO(dynamicStateInfo);
  dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicStateInfo.dynamicStateCount = 2;
  dynamicStateInfo.pDynamicStates = dynamicStates;
  return dynamicStateInfo;
}

VkPipelineVertexInputStateCreateInfo pe_vk_pipeline_get_default_vertex_input(){
  VkPipelineVertexInputStateCreateInfo vertexInputInfo;
  ZERO(vertexInputInfo);
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.pVertexBindingDescriptions = NULL;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = NULL;
  return vertexInputInfo;

}
void pe_vk_pipeline_create_layout() {

  VkPipelineLayoutCreateInfo pipelineLayoutInfo;
  ZERO(pipelineLayoutInfo);
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = NULL;
  pipelineLayoutInfo.pNext = NULL;
  pipelineLayoutInfo.flags = 0;

  VKVALID(vkCreatePipelineLayout(vk_device, &pipelineLayoutInfo, NULL,
                                 &pe_vk_pipeline_layout),
          "Can't create Pipeline Layout");
}

VkPipelineViewportStateCreateInfo pe_vk_pipeline_get_default_viewport(){

  VkViewport viewport;
  ZERO(viewport);
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)pe_vk_swch_extent.width;
  viewport.height = (float)pe_vk_swch_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkOffset2D offset;
  ZERO(offset);
  offset.x = 0;
  offset.y = 0;

  VkRect2D scissor;
  ZERO(scissor);
  scissor.offset = offset;
  scissor.extent = pe_vk_swch_extent;

  VkPipelineViewportStateCreateInfo viewportState;
  ZERO(viewportState);
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  return viewportState;
}

VkPipelineRasterizationStateCreateInfo pe_vk_pipeline_get_default_rasterization(){

  // rasterizer
  VkPipelineRasterizationStateCreateInfo rasterizer;
  ZERO(rasterizer);
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_NONE;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  return rasterizer;
}

VkPipelineInputAssemblyStateCreateInfo pe_vk_pipeline_get_default_input_assembly(){

  VkPipelineInputAssemblyStateCreateInfo inputAssembly;
  ZERO(inputAssembly);
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;
  return inputAssembly;
}

VkPipelineMultisampleStateCreateInfo pe_vk_pipeline_get_default_multisample(){

  // multisampling
  VkPipelineMultisampleStateCreateInfo multisampling;
  ZERO(multisampling);
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  return multisampling;
}

VkPipelineColorBlendStateCreateInfo pe_vk_pipeline_get_default_color_blend(){

  pe_vk_main_pipeline_info.color_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  pe_vk_main_pipeline_info.color_attachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending;
  ZERO(colorBlending);
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &pe_vk_main_pipeline_info.color_attachment;

  return colorBlending;

}

void pe_vk_pipeline_init() {
  VkVertexInputBindingDescription binding =
      pe_vk_vertex_get_binding_description();
  VkVertexInputAttributeDescription des = pe_vk_vertex_get_attribute();


  ZERO(pe_vk_main_pipeline_info);
  pe_vk_main_pipeline_info.vertex_input_state = pe_vk_pipeline_get_default_vertex_input();
  pe_vk_main_pipeline_info.rasterization_state = pe_vk_pipeline_get_default_rasterization();
  pe_vk_main_pipeline_info.dynamic_state = pe_vk_pipeline_get_default_dynamic_state();
  pe_vk_main_pipeline_info.viewport_state = pe_vk_pipeline_get_default_viewport();
  pe_vk_main_pipeline_info.input_assembly_state = pe_vk_pipeline_get_default_input_assembly();
  pe_vk_main_pipeline_info.multisample_state = pe_vk_pipeline_get_default_multisample();
  pe_vk_main_pipeline_info.color_blend_state = pe_vk_pipeline_get_default_color_blend();

  
  VkGraphicsPipelineCreateInfo pipeline_info;
  ZERO(pipeline_info);
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = 2;
  pipeline_info.pStages = pe_vk_shaders_stages_infos;//created in pe_vk_shader_load()
  pipeline_info.layout = pe_vk_pipeline_layout;//created in pe_vk_pipeline_create_layout()
  pipeline_info.renderPass = pe_vk_render_pass;//created in pe_vk_create_render_pass()

  pipeline_info.pVertexInputState = &pe_vk_main_pipeline_info.vertex_input_state;
  pipeline_info.pInputAssemblyState = &pe_vk_main_pipeline_info.input_assembly_state;
  pipeline_info.pViewportState = &pe_vk_main_pipeline_info.viewport_state;
  pipeline_info.pRasterizationState = &pe_vk_main_pipeline_info.rasterization_state;
  pipeline_info.pMultisampleState = &pe_vk_main_pipeline_info.multisample_state;
  pipeline_info.pColorBlendState = &pe_vk_main_pipeline_info.color_blend_state;
  pipeline_info.pDynamicState = &pe_vk_main_pipeline_info.dynamic_state;
  pipeline_info.pDepthStencilState = NULL;

  pipeline_info.subpass = 0;

  VKVALID(vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1,
                                    &pipeline_info, NULL, &pe_vk_pipeline),
                                    "Can't create pipeline");
}
