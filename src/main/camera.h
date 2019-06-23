//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_CAMERA_H
#define PAVONTHEGAME_CAMERA_H

#include <cglm.h>

typedef struct camera {
        mat4 projection;
        mat4 view;
}Camera;

Camera main_camera;
vec3 camera_up;
vec3 camera_front;
vec3 camera_position;
float camera_heigth_screen;
float camera_width_screen;

void init_camera();

#endif //PAVONTHEGAME_CAMERA_H
