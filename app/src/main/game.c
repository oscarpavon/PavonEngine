//
// Created by pavon on 6/24/19.
//

#include "game.h"
#include "input.h"

#include "gui.h"
#include <cglm.h>

#include "camera.h"

void handle_camera_rotation(){

}
mat4 tranlate_dot_rot;

void init_camera_local_transform(){
    mat4 camera_local_mat_position;
    glm_mat4_identity(camera_local_mat_position);

    glm_translate(camera_local_mat_position, (vec3){-0.4,-0.5,-3.5});

    init_vec3(0,-2,1.f, camera_position);
    update_look_at();

    glm_mat4_mul(camera_local_mat_position, main_camera.view, tranlate_dot_rot);

}
void init_game(){
    player.model = &new_level.models_array.models[0];
    glm_rotate(player.model->model_mat,180, (vec3){0,0,1});
    init_camera_local_transform();
}




void update_player_camera(){
    mat4 inverse_player_matrix;
    glm_mat4_inv(player.model->model_mat, inverse_player_matrix);

    glm_mat4_mul(tranlate_dot_rot,inverse_player_matrix,main_camera.view);

}

void update_game(){
    update_player_camera();
    if(button1.pressed){
        glm_translate(player.model->model_mat, (vec3){0,-0.01,0});
    }
    if(back_button.pressed){
        glm_translate(player.model->model_mat, (vec3){0,0.01,0});
    }
    if(virtual_joystick.pressed){
        virtual_joystick.position[0] = touch_position_x;
        virtual_joystick.position[1] = touch_position_y;
        LOGW("joystic pressed");
    }

}