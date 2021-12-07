#include "uniform_buffer.h"
#include <engine/engine.h>
#include "vk_buffer.h"

PEUniformBufferObject ubo;


void pe_vk_ubo_init(){
    ZERO(ubo);
    glm_mat4_identity(ubo.view);
    glm_mat4_identity(ubo.projection);
    glm_mat4_identity(ubo.model);
}

PEVKBufferCreateInfo pe_vk_uniform_buffer_create_buffer(size_t size){

        PEVKBufferCreateInfo info;
        ZERO(info);
        info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        info.size = size; 
    
        pe_vk_buffer_create(&info);
        return info;
}

void pe_vk_uniform_buffer_create(){
    VkDeviceSize buffer_size = sizeof(PEUniformBufferObject);

    array_init(&pe_vk_uniform_buffers,sizeof(VkBuffer),4);
    array_init(&pe_vk_uniform_buffers_memory,sizeof(VkDeviceMemory),4);


    for(int i = 0; i < 4 ; i++){
        //create buffer
        PEVKBufferCreateInfo info = pe_vk_uniform_buffer_create_buffer(sizeof(PEUniformBufferObject) * 100);
        array_add(&pe_vk_uniform_buffers,&info.buffer);
        array_add(&pe_vk_uniform_buffers_memory,&info.buffer_memory);


        array_add(&test_model->ubo_buffer,&info.buffer);
        array_add(&test_model->ubo_memoru,&info.buffer_memory);
        
        array_add(&test_model2->ubo_buffer,&info.buffer);
        array_add(&test_model2->ubo_memoru,&info.buffer_memory);

    }

     

    buffer_color = pe_vk_uniform_buffer_create_buffer(sizeof(PEColorShader));

}

void pe_vk_memory_copy(size_t size, VkDeviceMemory* memory, void* in_data){
    
    void* data;
    vkMapMemory(vk_device, *(memory), 0, size, 0, &data);
    memcpy(data, in_data, size);
    vkUnmapMemory(vk_device, *(memory));
}

void pe_vk_uniform_buffer_update(uint32_t image_index){


    PEUniformBufferObject pawn_ubo;
    PEUniformBufferObject rook_ubo;

    ZERO(pawn_ubo);
    ZERO(rook_ubo);

    glm_mat4_identity(pawn_ubo.model);
    glm_mat4_identity(rook_ubo.model);
    
    glm_rotate(pawn_ubo.model,0.002f,VEC3(0,1,0));
    glm_rotate(rook_ubo.model,0.002f,VEC3(1,0,0));

    glm_mat4_copy(main_camera.projection,pawn_ubo.projection);
    glm_mat4_copy(main_camera.view,pawn_ubo.view);
     
    glm_mat4_copy(main_camera.projection,rook_ubo.projection);
    glm_mat4_copy(main_camera.view,rook_ubo.view);

    pawn_ubo.projection[1][1] *= -1;
    rook_ubo.projection[1][1] *= -1;

    PEUniformBufferObject buffers[] = {pawn_ubo,rook_ubo};
    
    VkDeviceMemory* memory = array_get(&pe_vk_uniform_buffers_memory,image_index);
    
    pe_vk_memory_copy(sizeof(buffers),memory,buffers);





    VkDeviceMemory* pawn_memory = array_get(&test_model->ubo_memoru,image_index);
     
    pe_vk_memory_copy(sizeof(PEUniformBufferObject),pawn_memory,&pawn_ubo);


    
    VkDeviceMemory* rook_memory = array_get(&test_model->ubo_memoru,image_index);
     
    pe_vk_memory_copy(sizeof(PEUniformBufferObject),rook_memory,&rook_ubo);




    PEColorShader sh;
    ZERO(sh);
    sh.x = 0;
    sh.y = 0;
    sh.z = 1;

    pe_vk_memory_copy(sizeof(sh),&buffer_color.buffer_memory,&sh);
  
}
