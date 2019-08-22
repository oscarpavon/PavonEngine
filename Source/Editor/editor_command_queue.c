#include "editor_command_queue.h"


void editor_command_queue_init(){
    array_init(&array_editor_command_queue,sizeof(EditorCommand),30);

}

void editor_command_queue_udpate(){
    if(array_editor_command_queue.block)
        return;

    for (u8 i = 0; i < array_editor_command_queue.count; i++)
    {
        EditorCommand* new_command = array_get(&array_editor_command_queue,i);
        if(new_command && !new_command->done){
            new_command->command(new_command->command_text);
            new_command->done = true;
        }
    }
    int undone_jobs = 0;
    for (u8 i = 0; i < array_editor_command_queue.count; i++)
    {
        EditorCommand* new_command = array_get(&array_editor_command_queue,i);
        if(new_command && !new_command->done){
            undone_jobs ++;
        }
    }
    
    if(undone_jobs == 0)
        array_clean(&array_editor_command_queue);
}