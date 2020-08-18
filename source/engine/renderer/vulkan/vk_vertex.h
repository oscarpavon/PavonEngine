#include <engine/renderer/vulkan/vulkan.h>
#include <engine/array.h>

Array vertices;

VkVertexInputBindingDescription pe_vk_vertex_get_binding_description();
VkVertexInputAttributeDescription pe_vk_vertex_get_attribute();

VkBuffer pe_vk_model_create();

VkBuffer vertex_buffer;