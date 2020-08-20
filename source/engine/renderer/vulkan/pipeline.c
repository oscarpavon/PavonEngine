#include "pipeline.h"
#include <engine/renderer/vulkan/vulkan.h>
#include "shader_module.h"
#include <engine/macros.h>
#include "vk_vertex.h"
#include "descriptor_set.h"
#include "vk_descriptor_pool.h"
#include "uniform_buffer.h"

VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_LINE_WIDTH
    };



void pe_vk_pipeline_init(){
    


    VkVertexInputBindingDescription binding = pe_vk_vertex_get_binding_description();
    VkVertexInputAttributeDescription des = pe_vk_vertex_get_attribute();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    ZERO(vertexInputInfo);
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &binding;
    vertexInputInfo.vertexAttributeDescriptionCount = 1;
    vertexInputInfo.pVertexAttributeDescriptions = &des;


    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    ZERO(inputAssembly);
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;


    VkViewport viewport;
    ZERO(viewport);
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) pe_vk_swch_extent.width;
    viewport.height = (float) pe_vk_swch_extent.height;
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


    //rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer;
    ZERO(rasterizer);
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional


    //multisampling
    VkPipelineMultisampleStateCreateInfo multisampling;
    ZERO(multisampling);
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = NULL; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional


    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    ZERO(colorBlendAttachment);
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional


    VkPipelineColorBlendStateCreateInfo colorBlending;
    ZERO(colorBlending);
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    

    VkPipelineDynamicStateCreateInfo dynamicState;
    ZERO(dynamicState);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;



    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    ZERO(pipelineLayoutInfo);
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &pe_vk_descriptor_set_layout;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = NULL; // Optional
    pipelineLayoutInfo.pNext = NULL;
    pipelineLayoutInfo.flags = 0;

    vkCreatePipelineLayout(vk_device, &pipelineLayoutInfo, NULL, &pe_vk_pipeline_layout);


    pe_vk_create_render_pass();

    pe_vk_shader_load();

    VkGraphicsPipelineCreateInfo pipeline_info;
    ZERO(pipeline_info);
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = pe_vk_shaders_stages;

    pipeline_info.pVertexInputState = &vertexInputInfo;
    pipeline_info.pInputAssemblyState = &inputAssembly;
    pipeline_info.pViewportState = &viewportState;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = NULL;
    pipeline_info.pColorBlendState = &colorBlending;
    pipeline_info.pDynamicState = NULL;

    pipeline_info.layout = pe_vk_pipeline_layout;

    pipeline_info.renderPass = pe_vk_render_pass;
    pipeline_info.subpass = 0;

    
    vkCreateGraphicsPipelines(vk_device,VK_NULL_HANDLE,1,&pipeline_info, NULL, &pe_vk_pipeline);


}
    
