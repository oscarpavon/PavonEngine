#ifndef ENGINE_FILES_PARSER
#define ENGINE_FILES_PARSER

#include "array.h"

void load_level_elements_from_json(const char* json_file, int json_file_size);

void parse_gui_file(const char* json_file, int json_file_size);

#endif // !ENGINE_FILES_PARSER