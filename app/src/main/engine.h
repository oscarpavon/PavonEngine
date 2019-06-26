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


void init_engine();
void engine_loop();
void init_game_engine();

void init_models(ModelArray* array);
void draw_models(ModelArray* models);
void load_model_texture_to_gpu(ModelArray* models_array);

void init_model(struct Model* new_model);

void update_editor(); 

bool should_close;



#endif //PAVON_MOBILE_ENGINE_H
