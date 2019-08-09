//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_ENGINE_H
#define PAVON_ENGINE_H

#ifdef ANDROID
    #include <common.h>
    struct android_app* app;
#else
    #include <stdbool.h>
#endif

#include <signal.h>

#include "Engine/array.h"
#include "Engine/log.h"

#include "model.h"
#include "Engine/data.h"
#include "third_party/cglm/cglm.h"

#include "file_loader.h"
#include "Engine/files_parser.h"
#include "Engine/memory.h"
#include "gui.h"

#include <time.h>

#include "Engine/shader.h"

#include "Engine/components/components.h"
#include "Engine/animation/animation.h"

#include "camera.h"

#define VEC3(p1,p2,p3) (vec3){p1,p2,p3}

static const char* const level_folder = "../assets/Game/levels/";
static const char* const gui_folder = "../assets/gui/";

int action_pointer_id_count;
Array actions_pointers;
void add_action_function(void(*f)(void));

void init_engine();
void engine_loop();
void init_game_engine();

/* Draw array of engine elements, 
if in editor the shader need color multiplication uniform otherwise 
it not show it becouse color is multiplied per 0 ! */
void draw_elements(Array* elements);
void draw_simgle_model(struct Model * new_model);

void load_model_texture_to_gpu(Texture*);

void init_model_gl_buffers(struct Model* new_model);

void draw_editor_viewport(); 

void set_selected_element_transform(vec3 position, versor rotation);
void rotate_element(Element* element, versor quaternion);

void set_element_position(Element* element, vec3 position);

void update_viewport_size();

void init_static_gpu_vertex_buffer(Array* array, GLuint *id);
void init_static_gpu_index_buffer(Array* array, GLuint *id);
void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix);
void update_gpu_vertex_data(Array* array, GLuint id);

void new_empty_element();

/*Create new model in actual model array and you can use selected_model after */
void new_empty_model();

void select_last_element();

void add_element_with_gltf_path(const char* model_gltf_path);
void add_texture_to_selected_element_with_image_path(const char* image_path);

void load_model_to_array(Array* array, const char* path_model, const char* color_texture_path);

int load_and_initialize_simple_model(const char* model_gltf_path);

void update_translation(vec3 translation);

void load_simple_image(const char* path);

void test_elements_occlusion();

void duplicate_model_data(Model* destination , Model* source);

void new_empty_model_in_array(Array* array);

void check_meshes_distance();

//
// Global variables
//

bool should_close;

float frame_time;

bool game_initialized;

unsigned int element_id_count;

unsigned int components_id_count;

ComponentType current_loaded_component_type;

//
// Global array containers
//
Array components;
Array engine_native_models;

Array array_models_loaded;

Array array_hirarchical_level_of_detail;

//
// Global pointers
//
Element* selected_element;
Button* selected_button;
Model* selected_model;
Element* player1;
Element* player_start;

ComponentDefinition* current_component_selected;

Array* previous_models_array;
Array* current_textures_array;
Array* current_nodes_array;
Array* actual_model_array;
Array* actual_elements_array;

Array* actual_buttons_array;


//
/*Draw frame data */
//
Array frame_draw_elements;
Array models_for_test_occlusion;
Array array_static_meshes_pointers;
Array array_static_meshes_pointers_for_test_distance;
Array array_skinned_mesh_pointers;
Array array_skinned_mesh_for_distance_test;

//
//Paths data
//
Array texts;
Array textures_paths;


static inline void update_mvp(mat4 model, mat4 mvp_out){
    mat4 projection_view;
    glm_mul(main_camera.projection , main_camera.view, projection_view);
    glm_mul(projection_view , model , mvp_out);
}

#endif //PAVON_ENGINE_H
