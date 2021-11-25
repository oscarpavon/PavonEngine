
#include "vulkan.h"
#include <engine/vertex.h>
#include <engine/macros.h>
#include <engine/array.h>
#include "vk_memory.h"
#include <engine/array.h>
#include "vk_vertex.h"
#include "vk_buffer.h"

#include <engine/model.h>
#include <engine/engine.h>

VkVertexInputBindingDescription pe_vk_vertex_get_binding_description(){
    VkVertexInputBindingDescription binding;
    ZERO(binding);
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return binding;
}


VkVertexInputAttributeDescription pe_vk_vertex_get_attribute(){
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 0;

    attribute.format = VK_FORMAT_R32G32_SFLOAT;
    attribute.offset = offsetof(Vertex,postion);


    return attribute;
}


VkBuffer pe_vk_vertex_create_buffer(Array* vertices){
    PEVKBufferCreateInfo info;
    ZERO(info);
    info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    info.size = vertices->actual_bytes_size;
 
    pe_vk_buffer_create(&info);


    void* data;
    vkMapMemory(vk_device,info.buffer_memory,0,info.size,0,&data);
        memcpy(data,vertices->data,vertices->actual_bytes_size);
    vkUnmapMemory(vk_device,info.buffer_memory);

    return info.buffer;

}


VkBuffer pe_vk_vertex_create_index_buffer(Array* indices){
    VkBuffer buffer;

    PEVKBufferCreateInfo info;
    ZERO(info);
    info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    info.size = indices->actual_bytes_size;

    pe_vk_buffer_create(&info);
 
    void* data;
    vkMapMemory(vk_device,info.buffer_memory,0,info.size,0,&data);
        memcpy(data,indices->data,indices->actual_bytes_size);
    vkUnmapMemory(vk_device,info.buffer_memory);

    return info.buffer;
}


void pe_vk_model_create(){

    array_init(&vertices, sizeof(Vertex), 4);
    array_init(&model_indices, sizeof(uint16_t), 6);

    Vertex vertex1;
    Vertex vertex2;
    Vertex vertex3;
    Vertex vertex4;


    glm_vec3_copy((vec3){1.0f, 0.0f, 0.0f}, vertex1.postion);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, vertex2.postion);

    glm_vec3_copy((vec3){0.0f, 0.0f, 1.0f}, vertex3.postion);

    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, vertex4.postion);


    array_add(&vertices, &vertex1);
    array_add(&vertices, &vertex2);
    array_add(&vertices, &vertex3);
    array_add(&vertices, &vertex4);

    uint16_t indices_ar[] = {0,1,2,2,3,0};
    
    for(int i = 0; i < 6; i++){
        array_add(&model_indices,&indices_ar[i]);
    }

    actual_model_array = &array_models_loaded;
    pe_loader_model("/home/pavon/plane.glb");
    
     LOG("VK model loaded");
    

    vertex_buffer =  pe_vk_vertex_create_buffer(&selected_model->vertex_array);
    index_buffer =  pe_vk_vertex_create_index_buffer(&selected_model->index_array);

    //vertex_buffer =  pe_vk_vertex_create_buffer(&vertices);
    //index_buffer =  pe_vk_vertex_create_index_buffer(&model_indices);

}
