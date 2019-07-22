//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_INPUT_H
#define PAVONTHEGAME_INPUT_H

#include "utils.h"
typedef struct android_app android_app;
#ifdef ANDROID
#include <android/input.h>
int handle_input(android_app* app, AInputEvent* event);
#endif //def ANDROID

float touch_position_x;
float touch_position_y;

#endif //PAVONTHEGAME_INPUT_H
