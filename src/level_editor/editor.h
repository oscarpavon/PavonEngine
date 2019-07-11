#ifndef EDITOR_EDITOR
#define EDITOR_EDITOR

#include <cglm/cglm.h>
#include <cglm/quat.h>

#include "text.h"
#include "../engine.h"
#include "data.h"

#include "input.h"

#include "editor_shader.h"

#include "editor_gizmos.h"


typedef enum EditorMode{
    EDITOR_GRAB_MODE,
    EDITOR_DEFAULT_MODE,
    EDITOR_SCALE_MODE,
    EDITOR_ROTATE_MODE,
    EDITOR_NAVIGATE_MODE,
    EDITOR_CHANGING_MODE_MODE,
    EDITOR_TEXT_INPUT_MODE,
    EDITOR_PLAY_MODE
}EditorMode;

void init_editor();

void update_editor();

void get_element_status(Element* element);

void load_level_in_editor(const char*);

void list_directory_files();

void get_elements_in_editor_map();

void deselect_all();

extern void add_editor_element(const char* path_to_element);

void add_editor_texture(const char* image_path);

void change_to_editor_mode(EditorMode mode);

void clean_editor();

void rotate_editor_element(Element* element, float angle, vec3 axis);

void remove_selected_element();

void reload_editor();

void update_camera_aspect_ratio();

void duplicate_selected_element();

void add_editor_native_element(const char* native_element_name);

void play_game_standalone();

char* editor_mode_show_text;

enum EditorMode editor_mode;

float camera_velocity;

Array editor_elements;

Array selected_elements_id;


#endif