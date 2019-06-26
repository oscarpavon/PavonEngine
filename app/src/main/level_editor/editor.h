#ifndef EDITOR_EDITOR
#define EDITOR_EDITOR

#include <cglm/cglm.h>

void create_input_thread();
void init_editor();
void update_editor();

typedef struct Element{
    unsigned int id;
    vec3 position;
    struct Model* model;
    const char* model_path;
}Element;

Element* selected_element;

bool grab_mode;

void get_element_status(Element* element);
void save_data();
void load_level_in_editor();
void list_directory_files();


void add_element(const char* path_to_element);
#endif