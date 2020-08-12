#include <engine/renderer/vulkan/vulkan.h>
#include <engine/file_loader.h>

VkShaderModule pe_vk_shader_module_create(File* file){
    VkShaderModuleCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = file->size_in_bytes;
    info.pCode = (uint32_t)file->data;

    VkShaderModule module;
    vkCreateShaderModule(vk_device,&info,NULL,&module);
    return module;
}

void pe_vk_shader_load(){
    File new_file;
    load_file("/home/pavon/PavonEngine/NativeContent/shaders/shader_vk_vert.vert",&new_file);
    
    File new_file2;
    load_file("/home/pavon/PavonEngine/NativeContent/shaders/shader_vk_vert.vert",&new_file2);

    VkShaderModule module1 = pe_vk_shader_module_create(&new_file);
    VkShaderModule module2 = pe_vk_shader_module_create(&new_file2);


    VkPipelineShaderStageCreateInfo info1;
    info1.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info1.stage = VK_SHADER_STAGE_VERTEX_BIT;
    info1.module = module2;
    info1.pName = "main";

    
    VkPipelineShaderStageCreateInfo info2;
    info2.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    info2.stage = VK_SHADER_STAGE_VERTEX_BIT;
    info2.module = module1;
    info2.pName = "main";


    VkPipelineShaderStageCreateInfo shader_stages[] = {info1,info2};


}