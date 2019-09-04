//
// Created by pavon on 6/23/19.
//

#include "camera.h"
#include "utils.h"

void camera_init(CameraComponent* camera){
    camera_rotate_angle = 0;

    glm_mat4_identity(camera->view);
    glm_mat4_identity(camera->projection);

    init_vec3(1.0f, 0 ,  0.0f, camera->front);
    init_vec3(0.0f, 0.0f,  1.0f, camera->up);
    init_vec3(0,0,0, camera->position);

    vec3 look_pos;
    glm_vec3_add(camera->position, camera->front, look_pos);

    glm_lookat(camera->position, look_pos, camera->front , camera->view);

    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , camera->projection);

}

void camera_update(CameraComponent* camera){
    vec3 look_pos;
    glm_vec3_add(camera->position, camera->front, look_pos);

    glm_lookat(camera->position, look_pos, camera->up , camera->view);
}
