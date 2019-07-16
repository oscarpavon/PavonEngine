//
// Created by pavon on 6/24/19.
//

#include "game.h"
#include "input.h"

#include "gui.h"
#include <cglm.h>

#include "camera.h"

#include "engine.h"

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

void move_player_forward(){
    printf("Action pointer work\n");
}

void init_game(){    
    glm_rotate(player1->model->model_mat,180, (vec3){0,0,1});
    init_camera_local_transform();

    add_action_function(&move_player_forward);
}


void update_player_camera(){
    mat4 inverse_player_matrix;
    glm_mat4_inv(player1->model->model_mat, inverse_player_matrix);

    glm_mat4_mul(tranlate_dot_rot,inverse_player_matrix,main_camera.view);

}

void update_game(){
    update_player_camera(); 
}