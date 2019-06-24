//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_ENGINE_H
#define PAVON_MOBILE_ENGINE_H



#ifdef ANDROID
#include <common.h>
struct android_app* app;
#else
#include <stdbool.h>
#endif


void init_engine();
void engine_loop();

bool should_close;



#endif //PAVON_MOBILE_ENGINE_H
