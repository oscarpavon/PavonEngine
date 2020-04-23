#ifndef EDITOR_COMMAND_H
#define EDITOR_COMMAND_H
#include "../engine/numbers.h"

void parse_command(const char* command);
void text_input_mode();
char command_text_buffer[1000];
u8 command_character_count;

void system_command(const char* command, const char* argument);

int command_parse_from_command_line(int argc, char* argv[]);

void pe_editor_parse_cmd_char(unsigned char character);

#endif
