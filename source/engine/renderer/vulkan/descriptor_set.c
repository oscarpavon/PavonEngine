#include "descriptor_set.h"

#include "uniform_buffer.h"
#include <engine/array.h>
#include <engine/engine.h>

void pe_vk_descriptor_pool_create() {
  VkDescriptorPoolSize pool_size[2];
  ZERO(pool_size);
  pool_size[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size[0].descriptorCount = 4;
  pool_size[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size[1].descriptorCount = 4;

  VkDescriptorPoolCreateInfo info;
  ZERO(info);
  info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  info.poolSizeCount = 2;
  info.pPoolSizes = pool_size;
  info.maxSets = 8;

  VKVALID(
      vkCreateDescriptorPool(vk_device, &info, NULL, &pe_vk_descriptor_pool),
      "Can't create descriptor pool");
}

void pe_vk_create_descriptor_set_layout() {
  VkDescriptorSetLayoutBinding uniform;
  ZERO(uniform);
  uniform.binding = 0;
  uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uniform.descriptorCount = 1;
  uniform.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkDescriptorSetLayoutBinding all_binding[] = {uniform};

  VkDescriptorSetLayoutCreateInfo info;
  ZERO(info);
  info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  info.bindingCount = 1;
  info.pBindings = all_binding;

  VKVALID(vkCreateDescriptorSetLayout(vk_device, &info, NULL,
                                      &pe_vk_descriptor_set_layout),
          "Can't create Descriptor Set Layout");
}
void pe_vk_descriptor_update() {

  for (int i = 0; i < 2; i++) {

    VkDescriptorBufferInfo info;
    ZERO(info);
    VkBuffer *buffer = array_get(&pe_vk_uniform_buffers, i);
    info.buffer = *(buffer);
    info.offset = 0;
    info.range = sizeof(PUniformBufferObject);

    VkDescriptorSet *set = array_get(&pe_vk_descriptor_sets, i);

    VkWriteDescriptorSet des_write;
    ZERO(des_write);
    des_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    des_write.dstSet = *(set);
    des_write.dstBinding = 0;
    des_write.dstArrayElement = 0;
    des_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    des_write.descriptorCount = 1;
    des_write.pBufferInfo = &info;

    vkUpdateDescriptorSets(vk_device, 1, &des_write, 0, NULL);
  }
}
void pe_vk_descriptor_set_create() {

  VkDescriptorSetLayout layouts[2];

  ZERO(layouts);

  for (int i = 0; i < 2; i++) {
    layouts[i] = pe_vk_descriptor_set_layout;
  }

  array_init(&pe_vk_descriptor_sets, sizeof(VkDescriptorSet), 4);

  // resize because we need to allocate descriptor copy in array.data
  array_resize(&pe_vk_descriptor_sets, 4);

  // Allocation
  VkDescriptorSetAllocateInfo alloc_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .descriptorPool = pe_vk_descriptor_pool,
    .descriptorSetCount = 2,
    .pSetLayouts = layouts
  };

  vkAllocateDescriptorSets(vk_device, &alloc_info, pe_vk_descriptor_sets.data);

  pe_vk_descriptor_update();
}
