#include "descriptor_set.h"

#include <engine/engine.h>
#include "swap_chain.h"
#include <engine/array.h>
#include "uniform_buffer.h"
#include <engine/engine.h>

void pe_vk_descriptor_pool_create(){
    VkDescriptorPoolSize pool_size[2];
    ZERO(pool_size);
    pool_size[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size[0].descriptorCount = 100;
    pool_size[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size[1].descriptorCount = 100;

    VkDescriptorPoolCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.poolSizeCount = 2;
    info.pPoolSizes = pool_size;
    info.maxSets = 100;

    
    vkCreateDescriptorPool(vk_device,&info,NULL,&pe_vk_descriptor_pool);
}

void pe_vk_create_descriptor_set_layout(){
    VkDescriptorSetLayoutBinding uniform;
    ZERO(uniform);
    uniform.binding = 0;
    uniform.descriptorCount = 1;
    uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniform.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    
    VkDescriptorSetLayoutBinding color;
    ZERO(color);
    color.binding = 1;
    color.descriptorCount = 1;
    color.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    color.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    

    VkDescriptorSetLayoutBinding all_binding[] = {uniform,color};

    VkDescriptorSetLayoutCreateInfo info;
    ZERO(info);
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = 2;
    info.pBindings = all_binding;

    vkCreateDescriptorSetLayout(vk_device,&info,NULL,&pe_vk_descriptor_set_layout);

}

void pe_vk_descriptor_set_create() {

  VkDescriptorSetLayout layouts[100];

  ZERO(layouts);

  for (int i = 0; i < 100; i++) {
    layouts[i] = pe_vk_descriptor_set_layout;
  }

  array_init(&pe_vk_descriptor_sets, sizeof(VkDescriptorSet), 100);

  array_resize(&pe_vk_descriptor_sets,
               100); // resize because we need to allocate descriptor copy in array.data

  // Allocation
  VkDescriptorSetAllocateInfo alloc_info;
  ZERO(alloc_info);
  alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.descriptorPool = pe_vk_descriptor_pool;
  alloc_info.descriptorSetCount = 100;
  alloc_info.pSetLayouts = layouts;

  vkAllocateDescriptorSets(vk_device, &alloc_info, pe_vk_descriptor_sets.data);

  for (int i = 0; i < 4; i++) {

    for (int index_object = 0; index_object < 100; index_object++) {
      VkDescriptorBufferInfo info;
      ZERO(info);
      VkBuffer *buffer = array_get(&pe_vk_uniform_buffers, i);
      info.buffer = *(buffer);
      //info.offset = sizeof(PEUniformBufferObject) * index_object;
      info.offset = 0;
      info.range = sizeof(PEUniformBufferObject);

      VkDescriptorBufferInfo info2;
      ZERO(info2);
      info2.buffer = buffer_color.buffer;
      info2.offset = 0;
      info2.range = sizeof(buffer_color);

      VkDescriptorSet *set = array_get(&pe_vk_descriptor_sets, index_object);

      VkWriteDescriptorSet des_write[2];
      ZERO(des_write);
      des_write[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      des_write[0].dstSet = *(set);
      des_write[0].dstBinding = 0;
      des_write[0].dstArrayElement = 0;
      des_write[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      des_write[0].descriptorCount = 1;
      des_write[0].pBufferInfo = &info;

      des_write[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      des_write[1].dstSet = *(set);
      des_write[1].dstBinding = 1;
      des_write[1].dstArrayElement = 0;
      des_write[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      des_write[1].descriptorCount = 1;
      des_write[1].pBufferInfo = &info2;

      vkUpdateDescriptorSets(vk_device, 2, des_write, 0, NULL);
    }
  }
}
