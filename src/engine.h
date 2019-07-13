//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_ENGINE_H
#define PAVON_MOBILE_ENGINE_H

#include "vector.h"

#ifdef ANDROID
#include <common.h>
struct android_app* app;
#else
#include <stdbool.h>
#endif

#include <cglm.h>
#include "camera.h"
#include "file_loader.h"
#include "Engine/files_parser.h"

typedef enum ElementType{
    ELEMENT_TYPE_CAMERA = 1,
    ELEMENT_TYPE_PLAYER_START = 2,
    ELEMENT_TYPE_COLLIDER = 3,
    ELEMENT_TYPE_MODEL = 4,
    ELEMENT_TYPE_PLAYER_CONTROLLER = 5
}ElementType;

typedef struct Element{
    unsigned int id;
    ElementType type;
    char name[20];
    vec3 position;
    versor rotation;
    struct Model* model;
    char model_path[20];
    char texture_path[20];
    short int duplicated_of_id;
    bool selected;
    bool has_HLOD;
    bool has_LOD;
    unsigned int model_id;
}Element;

typedef struct PlayerStart{
    vec3 position;
    versor rotation; 
}PlayerStart;

typedef enum DataType{
    DATA_TYPE_LEVEL = 0,
    DATA_TYPE_GUI = 1,
    DATA_TYPE_HEADER = 2
}DataType;

void init_engine();
void engine_loop();
void init_game_engine();

void init_models(ModelArray* array);
void draw_elements(Array* elements);
void draw_simgle_model(struct Model * new_model);
void load_models_texture_to_gpu(ModelArray* models_array);
void load_model_texture_to_gpu(struct Model*);

void init_model(struct Model* new_model);

void update_editor(); 

void set_selected_element_transform(vec3 position, versor rotation);

void set_element_position(Element* element, vec3 position);

void update_viewport_size();

void init_static_gpu_vertex_buffer(VertexArray* array, GLuint *id);
void update_draw_vertices(GLuint shader, GLuint buffer);

extern void new_empty_element();

extern void new_empty_model();

extern void select_last_element();

void add_element_with_model_path(const char* model_gltf_path);
void add_texture_to_selected_element_with_image_path(const char* image_path);

bool should_close;

static inline void update_mvp(mat4 model, mat4 mvp_out){
    mat4 projection_view;
    glm_mul(main_camera.projection , main_camera.view, projection_view);
    glm_mul(projection_view , model , mvp_out);
}

unsigned int element_id_count;
unsigned int model_id_count;

Element* selected_element;
Model* selected_model;
Element* player1;
Element* player_start;

ModelArray* actual_model_array;
Array* actual_elements_array;
ModelArray* actual_LOD_models_array;
GLuint actual_standard_shader;
Array* actual_buttons_array;


static inline init_shader(GLuint shader, GLuint vertex_shader, GLuint fragment_shader){
    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);
    glLinkProgram(shader);
}
#endif //PAVON_MOBILE_ENGINE_H
