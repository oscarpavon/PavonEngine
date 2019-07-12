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

typedef struct CameraElement{
    vec3 position;
    versor rotation;
}CameraElement;

Camera main_camera;
vec3 camera_up;
vec3 camera_front;
vec3 camera_position;
float camera_heigth_screen;
float camera_width_screen;
versor camera_rotation;

void init_camera();
void update_look_at();

bool move_camera_input;

#endif //PAVONTHEGAME_CAMERA_H
