
#include "vulkan.h"
#include <engine/vertex.h>
#include <engine/macros.h>
#include <engine/array.h>
#include "vk_memory.h"



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

    void* data;
    vkMapMemory(vk_device,memory,0,info.size,0,&data);
        memcpy(data,vertices->data,vertices->actual_bytes_size);
    vkUnmapMemory(vk_device,memory);

    return buffer;

}