
#include "vk_vertex.h"
#include "vk_buffer.h"
#include "vk_memory.h"
#include "vulkan.h"
#include <engine/array.h>
#include <engine/macros.h>
#include <engine/vertex.h>

#include <engine/engine.h>
#include <engine/model.h>

#include <../demos/chess/chess.h>

VkVertexInputBindingDescription pe_vk_vertex_get_binding_description() {
  VkVertexInputBindingDescription binding;
  ZERO(binding);
  binding.binding = 0;
  binding.stride = sizeof(Vertex);
  binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return binding;
}

void pe_vk_vertex_get_attribute(PVertexAtrributes* attributes) {

  if (attributes->position) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 0;

    attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute.offset = offsetof(Vertex, position);
    array_add(&attributes->attributes_descriptions, &attribute);
  }
  if (attributes->color) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 1;

    attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute.offset = offsetof(Vertex, color);
    array_add(&attributes->attributes_descriptions, &attribute);
  }
  if (attributes->normal) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 2;

    attribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute.offset = offsetof(Vertex, normal);
    array_add(&attributes->attributes_descriptions, &attribute);
  }
  if (attributes->uv) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 3;

    attribute.format = VK_FORMAT_R32G32_SFLOAT;
    attribute.offset = offsetof(Vertex, uv);
    array_add(&attributes->attributes_descriptions, &attribute);
  }
  if (attributes->joint) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 4;

    attribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attribute.offset = offsetof(Vertex, joint);
    array_add(&attributes->attributes_descriptions, &attribute);
  }
  if (attributes->weight) {
    VkVertexInputAttributeDescription attribute;
    ZERO(attribute);

    attribute.binding = 0;
    attribute.location = 5;

    attribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attribute.offset = offsetof(Vertex, weight);
    array_add(&attributes->attributes_descriptions, &attribute);
  }

}

VkBuffer pe_vk_vertex_create_buffer(Array *vertices) {
  PEVKBufferCreateInfo info;
  ZERO(info);
  info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  info.size = vertices->actual_bytes_size;

  pe_vk_buffer_create(&info);

  void *data;
  vkMapMemory(vk_device, info.buffer_memory, 0, info.size, 0, &data);
  memcpy(data, vertices->data, vertices->actual_bytes_size);
  vkUnmapMemory(vk_device, info.buffer_memory);

  return info.buffer;
}

VkBuffer pe_vk_vertex_create_index_buffer(Array *indices) {
  VkBuffer buffer;

  PEVKBufferCreateInfo info;
  ZERO(info);
  info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  info.size = indices->actual_bytes_size;

  pe_vk_buffer_create(&info);

  void *data;
  vkMapMemory(vk_device, info.buffer_memory, 0, info.size, 0, &data);
  memcpy(data, indices->data, indices->actual_bytes_size);
  vkUnmapMemory(vk_device, info.buffer_memory);

  return info.buffer;
}

void pe_vk_models_create() {

  actual_model_array = &array_models_loaded;
  pe_loader_model("/sdcard/Download/chess/floor.glb");

  test_model = selected_model;

  test_model->vertex_buffer =
      pe_vk_vertex_create_buffer(&selected_model->vertex_array);
  test_model->index_buffer =
      pe_vk_vertex_create_index_buffer(&selected_model->index_array);

  pe_vk_create_uniform_buffers(test_model);
  pe_vk_descriptor_pool_create(test_model);
  pe_vk_descriptor_set_create(test_model);

  pe_loader_model("/sdcard/Download/chess/peon.glb");
  test_model2 = selected_model;

  test_model2->vertex_buffer =
      pe_vk_vertex_create_buffer(&selected_model->vertex_array);
  test_model2->index_buffer =
      pe_vk_vertex_create_index_buffer(&selected_model->index_array);


  pe_vk_create_uniform_buffers(test_model2);
  pe_vk_descriptor_pool_create(test_model2);
  pe_vk_descriptor_set_create(test_model2);
}
