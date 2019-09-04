//
// Created by pavon on 6/23/19.
//

#ifndef PAVONTHEGAME_CAMERA_H
#define PAVONTHEGAME_CAMERA_H

#include "../ThirdParty/cglm/cglm.h"

#include "components/components.h"

CameraComponent main_camera;
CameraComponent saved_camera;

float camera_heigth_screen;
float camera_width_screen;
versor camera_rotation;

void camera_init(CameraComponent* camera);

void camera_update(CameraComponent* camera);
bool move_camera_input;

float camera_rotate_angle;

#endif //PAVONTHEGAME_CAMERA_H
