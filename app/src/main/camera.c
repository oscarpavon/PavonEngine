//
// Created by pavon on 6/23/19.
//

#include "camera.h"
#include "utils.h"

void init_camera(){
    
    glm_mat4_identity(main_camera.view);
    glm_mat4_identity(main_camera.projection);

    init_vec3(0.0f, -1.0f, 0.0f, camera_front);
    init_vec3(0.0f, 0.0f,  1.0f, camera_up);
    init_vec3(0,-2,1.f, camera_position);

    vec3 look_pos;
    glm_vec3_add(camera_position, camera_front, look_pos);

    glm_lookat(camera_position, look_pos, camera_up , main_camera.view);

    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , main_camera.projection);

}