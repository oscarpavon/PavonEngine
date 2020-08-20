#include "uniform_buffer.h"
#include <engine/engine.h>

void pe_vk_uniform_buffer_create(){
    VkDeviceSize buffer_size = sizeof(PEUniformBufferObject);

    array_init(&pe_vk_uniform_buffers,sizeof(VkBuffer),4);
    array_init(&pe_vk_uniform_buffers_memory,sizeof(VkDeviceMemory),4);

    
    for(int i = 0; i< 4 ; i++){
        //create buffer
        
    }

}