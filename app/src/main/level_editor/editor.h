#ifndef EDITOR_EDITOR
#define EDITOR_EDITOR

#include <cglm/cglm.h>

void create_input_thread();
void init_editor();
void update_editor();

typedef struct Element{
    unsigned int id;
    char name;
    vec3 position;
    struct Model* model;
    char* model_path;
    char* texture_path;
    bool selected;
}Element;

Element* selected_element;

typedef enum {
    GRAB_MODE,
    DEFAULT_MODE,
    SCALE_MODE,
    ROTATE_MODE,
    NAVIGATE_MODE
}Mode;



void get_element_status(Element* element);
void save_data();
void load_level_in_editor();
void list_directory_files();

void get_elements_in_editor_map();


void add_editor_element(const char* path_to_element);
void add_editor_texture(const char* image_path);

const char* editor_mode_show_text;

Mode editor_mode;

#endif