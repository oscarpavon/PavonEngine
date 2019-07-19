//
// Created by pavon on 6/23/19.
//

#include "camera.h"
#include "utils.h"

void init_camera(){
    camera_rotate_angle = 0;

    glm_mat4_identity(main_camera.view);
    glm_mat4_identity(main_camera.projection);

    init_vec3(1.0f, 0 ,  0.0f, main_camera.front);
    init_vec3(0.0f, 0.0f,  1.0f, main_camera.up);
    init_vec3(0,0,0, main_camera.position);

    vec3 look_pos;
    glm_vec3_add(main_camera.position, main_camera.front, look_pos);

    glm_lookat(main_camera.position, look_pos, main_camera.front , main_camera.view);

    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , main_camera.projection);

}

void update_look_at(){
    vec3 look_pos;
    glm_vec3_add(main_camera.position, main_camera.front, look_pos);

    glm_lookat(main_camera.position, look_pos, main_camera.up , main_camera.view);
}