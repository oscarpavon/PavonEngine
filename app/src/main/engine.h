//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_ENGINE_H
#define PAVON_MOBILE_ENGINE_H


#include <common.h>

void init_engine();
void engine_loop();

bool should_close;

#ifdef ANDROID
struct android_app* app;
#endif

#endif //PAVON_MOBILE_ENGINE_H
