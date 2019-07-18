#ifndef ENGINE_FILES_PARSER
#define ENGINE_FILES_PARSER

#include <stddef.h>
#include "array.h"

void load_level_elements_from_json(const char* json_file, size_t json_file_size, struct Array* out_element);

void parse_gui_file(const char* json_file, size_t json_file_size, struct Array* out_element);

#endif // !ENGINE_FILES_PARSER