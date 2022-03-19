//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_INPUT_H
#define PAVONTHEGAME_INPUT_H

#include <engine/utils.h>
typedef struct android_app android_app;

#include <android/input.h>
int handle_input(android_app* app, AInputEvent* event);

float touch_position_x;
float touch_position_y;

#endif //PAVONTHEGAME_INPUT_H
