//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_ENGINE_H
#define PAVON_ENGINE_H

#include "vector.h"

#ifdef ANDROID
#include <common.h>
struct android_app* app;
#else
#include <stdbool.h>
#endif

#include "Engine/data.h"
#include <cglm.h>
#include "camera.h"
#include "file_loader.h"
#include "Engine/files_parser.h"
#include "Engine/memory.h"
#include "gui.h"

#include <time.h>

#include "shader.h"

#include "Engine/components/components.h"

#define VEC3(p1,p2,p3) (vec3){p1,p2,p3}

static const char* const level_folder = "../Game/levels/";
static const char* const gui_folder = "../assets/gui/";

int action_pointer_id_count;
Array actions_pointers;
void add_action_function(void(*f)(void));

void init_engine();
void engine_loop();
void init_game_engine();

void init_models(Array* array);

/* Draw array of engine elements, 
if in editor the shader need color multiplication uniform otherwise 
it not show it becouse color is multiplied per 0 ! */
void draw_elements(Array* elements);
void draw_simgle_model(struct Model * new_model);
void load_models_texture_to_gpu(Array* models_array);
void load_model_texture_to_gpu(struct Model*);

void init_model_gl_buffers(struct Model* new_model);

void update_editor(); 

void set_selected_element_transform(vec3 position, versor rotation);

void set_element_position(Element* element, vec3 position);

void update_viewport_size();

void init_static_gpu_vertex_buffer(VertexArray* array, GLuint *id);
void init_static_gpu_index_buffer(IndexArray* array, GLuint *id);
void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix);
void update_gpu_vertex_data(VertexArray* array, GLuint id);

void new_empty_element();

/*Create new model in actual model array and you can use selected_model after */
void new_empty_model();

void select_last_element();

void add_element_with_model_path(const char* model_gltf_path);
void add_texture_to_selected_element_with_image_path(const char* image_path);

void load_model_to_array(Array* array, const char* path_model, const char* color_texture_path);

void update_translation(vec3 translation);


bool should_close;

static inline void update_mvp(mat4 model, mat4 mvp_out){
    mat4 projection_view;
    glm_mul(main_camera.projection , main_camera.view, projection_view);
    glm_mul(projection_view , model , mvp_out);
}

unsigned int element_id_count;
unsigned int model_id_count;
unsigned int components_id_count;

Element* selected_element;
Button* selected_button;
Model* selected_model;
Element* player1;
Element* player_start;

Array* actual_model_array;
Array* actual_elements_array;
Array* actual_LOD_models_array;
GLuint actual_standard_fragment_shader;
Array* actual_buttons_array;


Array components;
Array engine_native_models;

Array frame_draw_elements;

float frame_time;

static int shader_count = 0;
GLuint static inline create_engine_shader(GLuint vertex, GLuint fragment){
    shader_count++;
    GLuint new_shader = glCreateProgram();
    glAttachShader(new_shader, vertex);
    glAttachShader(new_shader, fragment);
    glLinkProgram(new_shader);
    return new_shader;
}

#endif //PAVON_ENGINE_H
