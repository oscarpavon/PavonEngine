#include "draw.h"
#include "commands.h"
#include "pipeline.h"

void pe_vk_draw(int i){
    VkCommandBuffer* cmd_buffer = array_get(&pe_vk_command_buffers,i);


    vkCmdBindPipeline(*(cmd_buffer),VK_PIPELINE_BIND_POINT_GRAPHICS,pe_vk_pipeline);
    vkCmdDraw(*(cmd_buffer), 3, 1, 0, 0);
    


}