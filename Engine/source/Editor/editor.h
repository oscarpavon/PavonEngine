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

#include "export_model.h"

typedef enum EditorMode{
    EDITOR_GRAB_MODE,
    EDITOR_DEFAULT_MODE,
    EDITOR_SCALE_MODE,
    EDITOR_ROTATE_MODE,
    EDITOR_NAVIGATE_MODE,
    EDITOR_CHANGING_MODE_MODE,
    EDITOR_PLAY_MODE,
    EDITOR_MODE_GUI_EDITOR,
    EDITOR_SUB_MODE_TEXT_INPUT,
    EDITOR_SUB_MODE_NULL,
    EDITOR_SUB_MODE_GRAB,
    EDITOR_SUB_MODE_SCALE,
    EDITOR_SUB_MODE_ROTATE
}EditorMode;

void init_editor();

void draw_editor_viewport();

void get_element_status(Element* element);

void load_level_in_editor(const char*);

void list_directory_files();

void deselect_all();

void change_to_editor_mode(EditorMode mode);

void clean_editor();

void rotate_editor_element(Element* element, float angle, vec3 axis);

void rotate_editor_selected_element_with_quaternion(versor quaternion);

void remove_selected_element();

void reload_editor();

void update_camera_aspect_ratio();

void duplicate_selected_element();

void add_editor_native_element(const char* native_element_name);

void play_game_standalone();

void editor_add_HLOD_element();

void editor_generate_and_add_cube_element();

struct timespec diff(struct timespec start, struct timespec end);

char* editor_mode_show_text;
char* editor_sub_mode_text;

enum EditorMode editor_mode;
enum EditorMode editor_sub_mode;

float camera_velocity;

Array editor_elements;

Array selected_elements_id;

char opened_file_name[20];

bool controlling_camera_component;

char editing_blender_file_path[30];
bool is_editing_blender_file;

#endif