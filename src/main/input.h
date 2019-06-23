//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_INPUT_H
#define PAVONTHEGAME_INPUT_H

#include "utils.h"
#include <android/input.h>

int32_t handle_input(struct android_app* app, AInputEvent* event);

#endif //PAVONTHEGAME_INPUT_H
