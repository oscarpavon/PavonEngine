#include <engine/renderer/vulkan/vulkan.h>
#include <engine/array.h>

VkCommandPool pe_vk_commands_pool;

Array pe_vk_command_buffers;
void pe_vk_command_init();
void pe_vk_commands_pool_init();
