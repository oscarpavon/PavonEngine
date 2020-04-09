#ifndef EDITOR_COMMAND_H
#define EDITOR_COMMAND_H
#include "../engine/numbers.h"

void parse_command(const char* command);
void text_input_mode();
char command_text_buffer[100];
u8 command_character_count;

void system_command(const char* command, const char* argument);


#endif
