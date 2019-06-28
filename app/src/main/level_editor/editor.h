#ifndef EDITOR_EDITOR
#define EDITOR_EDITOR

#include <cglm/cglm.h>
#include <cglm/quat.h>

#include "text.h"
#include "../engine.h"

void create_input_thread();
void init_editor();
void update_editor();



Element* selected_element;

typedef enum {
    EDITOR_GRAB_MODE,
    EDITOR_DEFAULT_MODE,
    EDITOR_SCALE_MODE,
    EDITOR_ROTATE_MODE,
    EDITOR_NAVIGATE_MODE,
    EDITOR_CHANGING_MODE_MODE
}EditorMode;



void get_element_status(Element* element);
void save_data();
void load_level_in_editor();
void list_directory_files();

void get_elements_in_editor_map();


void add_editor_element(const char* path_to_element);
void add_editor_texture(const char* image_path);

void change_to_editor_mode(EditorMode mode);

void clean_editor();

void rotate_editor_element(Element* element, float angle, vec3 axis);

void remove_selected_element();

const char* editor_mode_show_text;

EditorMode editor_mode;

TextMenu editor_element_list_menu;

bool can_draw_gizmos;
bool can_draw_skeletal_bones;

#endif