//
// Created by pavon on 6/23/19.
//

#ifndef ENGINE_GUI_H
#define ENGINE_GUI_H

#include "utils.h"
#include "array.h"

#include "types.h"


#define POSITION_RELATIVE_LEFT_BOTTON 0
#define POSITION_RELATIVE_LEFT_TOP 1

typedef struct Button{
    char name[20];
    bool pressed;
    bool released;

    vec2 size;
    vec2 position;
    unsigned short int relative_to;
    vec2 original_position;

    u32 shader;

    int action_function_id;
}Button;


typedef struct Menu{
    Array buttons;
}Menu;

void two_dimension_screen_space_send_matrix(u32 shader_id, vec2 size, vec2 position);

void init_gui();
void draw_button();
void draw_gui();
void draw_loading_screen();
void update_button_matrix(u32 shader_id, vec2 size, vec2 position);
void update_user_iterface_status();

void new_empty_button();

void load_gui(const char* name);

float pixel_size_x;
float pixel_size_y;

u32 UI_plane_vertex_buffer_id;

#endif //ENGINE_GUI_H
