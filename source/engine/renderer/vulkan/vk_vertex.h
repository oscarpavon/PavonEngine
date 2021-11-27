#include <engine/renderer/vulkan/vulkan.h>
#include <engine/array.h>


VkVertexInputBindingDescription pe_vk_vertex_get_binding_description();
VkVertexInputAttributeDescription pe_vk_vertex_get_attribute();

void pe_vk_model_create();

VkBuffer vertex_buffer;
VkBuffer index_buffer;
