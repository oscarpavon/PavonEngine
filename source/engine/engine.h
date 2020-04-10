//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_ENGINE_H
#define PAVON_ENGINE_H

#include "interruptions.h"
#include "array.h"
#include "log.h"
#include "threads.h"

#include "input.h"

#include "vertex.h"

#include "model.h"
#include "types.h"
#include "../ThirdParty/cglm/cglm.h"

#include "file_loader.h"
#include "files_parser.h"
#include "memory.h"
#include "gui.h"

#include "time.h"

#include "shader.h"

#include "components/components.h"
#include "animation/animation.h"

#include "camera.h"

#include "Renderer/OpenGL/opengl_es2.h"

#include "Collision/collision.h"
#include "content_manager.h"

#include "text_renderer.h"

#include "audio/audio_engine.h"

#include "windows_manager.h"

#define VEC3(p1,p2,p3) (vec3){p1,p2,p3}
#define COLOR(color) color[0],color[1],color[2],color[3]

void engine_init();
void engine_init_render();
void engine_loop();
void init_game_engine();

void engine_program_main_loop(void(*program_loop)(void),EngineWindow* program_window);

void add_action_function(void(*f)(void));

/* Draw array of engine elements, 
if in editor the shader need color multiplication uniform otherwise 
it not show it becouse color is multiplied per 0 ! */
void engine_draw_elements(Array* elements);
void draw_simgle_model(struct Model * new_model);

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

void engine_add_element(u32 models_loaded);

void add_element_with_model_path(const char* model_gltf_path);
void add_texture_to_selected_element_with_image_path(const char* image_path);

void load_model_to_array(Array* array, const char* path_model, const char* color_texture_path);

int load_and_initialize_simple_model(const char* model_gltf_path);

//Transform
void update_translation(vec3 translation);
void update_scale(vec3 translation);
//*******

void load_simple_image(const char* path);

void test_elements_occlusion();

void duplicate_model_data(Model* destination , Model* source);

void new_empty_model_in_array(Array* array);

void check_meshes_distance();

void engine_add_texture_from_memory_to_selected_element(void* data, u32 size);
static const char* const level_folder = "../assets/Game/levels/";
static const char* const gui_folder = "../assets/gui/";

//
// Global variables
//
EngineThread thread_render;

bool engine_client_render_thread_initialized;

typedef struct RenderThread{
	void(*draw)(void);
	void(*init)(void);
	void(*finish)(void);
};

void(*engine_user_render_thread_draw)(void);
void(*engine_user_render_thread_init)(void);
void(*engine_user_render_thread_finish)(void);

bool engine_running;

bool engine_initialized;

bool should_close;

float frame_time;

bool game_initialized;

unsigned int element_id_count;

unsigned int components_id_count;

ComponentType current_loaded_component_type;

int action_pointer_id_count;

u8 FPS;
//
// Global array containers
//
Array components;
Array engine_native_models;

Array array_models_loaded;

Array array_hirarchical_level_of_detail;

Array array_render_thread_init_commmands;
Array array_render_thread_commands;


Array actions_pointers;;

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
