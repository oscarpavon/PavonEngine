#ifndef EDITOR_EDITOR
#define EDITOR_EDITOR

#include <cglm/cglm.h>
#include <cglm/quat.h>

#include "text.h"
#include "../engine.h"
#include "data.h"

#include "input.h"

#include "editor_shader.h"

void create_input_thread();
void init_editor();
void update_editor();



Element* selected_element;

typedef enum EditorMode{
    EDITOR_GRAB_MODE,
    EDITOR_DEFAULT_MODE,
    EDITOR_SCALE_MODE,
    EDITOR_ROTATE_MODE,
    EDITOR_NAVIGATE_MODE,
    EDITOR_CHANGING_MODE_MODE,
    EDITOR_TEXT_INPUT_MODE
}EditorMode;



void get_element_status(Element* element);

void load_level_in_editor(const char*);
void list_directory_files();

void get_elements_in_editor_map();

void deselect_all();

void add_editor_element(const char* path_to_element);

void add_editor_texture(const char* image_path);

void change_to_editor_mode(EditorMode mode);

void clean_editor();

void rotate_editor_element(Element* element, float angle, vec3 axis);

void remove_selected_element();

void reload_editor();

void update_camera_aspect_ratio();

void duplicate_selected_element();

const char* editor_mode_show_text;

enum EditorMode editor_mode;

struct TextMenu editor_element_list_menu;

bool can_draw_gizmos;
bool draw_translate_gizmo;
bool draw_rotate_gizmo;
bool can_draw_skeletal_bones;

float camera_velocity;


Array editor_elements;

unsigned int element_id_count;

#endif