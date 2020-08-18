
#include "vulkan.h"
#include <engine/vertex.h>
#include <engine/macros.h>
#include <engine/array.h>
#include "vk_memory.h"
#include <engine/array.h>
#include "vk_vertex.h"




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
    VkBuffer buffer;
    VkBufferCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = vertices->count * sizeof(struct Vertex);
    info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(vk_device,&info,NULL,&buffer);


    VkMemoryRequirements requirement = pe_vk_memory_get_requirements(buffer);
    VkDeviceMemory memory = pe_vk_memory_allocate(requirement);

    vkBindBufferMemory(vk_device, buffer, memory, 0);

    void* data;
    vkMapMemory(vk_device,memory,0,info.size,0,&data);
        memcpy(data,vertices->data,vertices->actual_bytes_size);
    vkUnmapMemory(vk_device,memory);

    return buffer;

}


VkBuffer pe_vk_vertex_create_index_buffer(Array* indices){
    VkDeviceSize buffer_size = indices->actual_bytes_size;
    
    VkBuffer buffer;

    VkBufferCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = indices->actual_bytes_size;
    info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(vk_device,&info,NULL,&buffer);

    index_buffer = buffer;

    VkMemoryRequirements requirement = pe_vk_memory_get_requirements(buffer);
    VkDeviceMemory memory = pe_vk_memory_allocate(requirement);

    vkBindBufferMemory(vk_device, buffer, memory, 0);

    void* data;
    vkMapMemory(vk_device,memory,0,info.size,0,&data);
        memcpy(data,indices->data,indices->actual_bytes_size);
    vkUnmapMemory(vk_device,memory);
}


VkBuffer pe_vk_model_create(){

   array_init(&vertices, sizeof(Vertex), 4);
    array_init(&indices, sizeof(uint16_t), 6);

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
        array_add(&indices,&indices_ar[i]);
    }


      
    VkBuffer buffer =  pe_vk_vertex_create_buffer(&vertices);

    vertex_buffer = buffer;

    pe_vk_vertex_create_index_buffer(&indices);



    return buffer;

}