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


void pe_vk_uniform_buffer_create(){
    VkDeviceSize buffer_size = sizeof(PEUniformBufferObject);

    array_init(&pe_vk_uniform_buffers,sizeof(VkBuffer),4);
    array_init(&pe_vk_uniform_buffers_memory,sizeof(VkDeviceMemory),4);

    
    for(int i = 0; i< 4 ; i++){
        //create buffer
        PEVKBufferCreateInfo info;
        ZERO(info);
        info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        info.size = sizeof(PEUniformBufferObject);
    
        pe_vk_buffer_create(&info);

        array_add(&pe_vk_uniform_buffers,&info.buffer);
        array_add(&pe_vk_uniform_buffers_memory,&info.buffer_memory);

    }


    pe_vk_ubo_init();
}

void pe_vk_uniform_buffer_update(uint32_t image_index){
  
    vec3 up;
    vec3 front;
    vec3 position;

    init_vec3(0.0f, 1.0f ,  0.0f, up);
    init_vec3(0.0f, 0.0f,  1.0f, front);
    init_vec3(1.f, 10.f, 0.f, position);
    
    glm_rotate(ubo.model,0.002f,VEC3(1,0,0));


    glm_lookat(position, VEC3(0,0,0), up , ubo.view);

    glm_perspective(45.f, 1260 / 720, 0.001f,
                    5000.f, ubo.projection);

    ubo.projection[1][1] *= -1;
    
    VkDeviceMemory* memory = array_get(&pe_vk_uniform_buffers_memory,image_index);

    void* data;
    vkMapMemory(vk_device, *(memory), 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(vk_device, *(memory));
  
  
}
