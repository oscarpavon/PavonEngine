#ifndef EDITOR_COMMAND_H
#define EDITOR_COMMAND_H

void parse_command(const char* command);
void text_input_mode();
char command_text_buffer[100];

void system_command(const char* command, const char* argument);

#endif