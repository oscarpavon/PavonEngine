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

typedef struct Element{
    unsigned int id;
    char* name;
    vec3 position;
    versor rotation;
    struct Model* model;
    char* model_path;
    char* texture_path;
    short int duplicated_of_id;   
}Element;

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

bool should_close;



#endif //PAVON_MOBILE_ENGINE_H
