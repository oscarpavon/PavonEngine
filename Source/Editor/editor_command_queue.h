#ifndef EDITOR_COMMAND_QUEUE
#define EDITOR_COMMAND_QUEUE
#include "../Engine/array.h"

typedef struct EditorCommand{
    bool done;
    void* data;
    char command_text[100];
    void (*command)(void*);
}EditorCommand;

Array array_editor_command_queue;

void editor_command_queue_udpate();
void editor_command_queue_init();

#endif