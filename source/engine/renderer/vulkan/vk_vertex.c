
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

    attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
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


    actual_model_array = &array_models_loaded;
    pe_loader_model("/home/pavon/chess/rook.glb");
    
    test_model = selected_model;
   
    test_model->vertex_buffer =  pe_vk_vertex_create_buffer(&selected_model->vertex_array);
    test_model->index_buffer =  pe_vk_vertex_create_index_buffer(&selected_model->index_array);


    pe_loader_model("/home/pavon/chess/pawn.glb");
    test_model2 = selected_model;

    test_model2->vertex_buffer =  pe_vk_vertex_create_buffer(&selected_model->vertex_array);
    test_model2->index_buffer =  pe_vk_vertex_create_index_buffer(&selected_model->index_array);

}
