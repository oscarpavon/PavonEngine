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

    init_vec3(0,-2,1.f, main_camera.position);
    update_look_at();

    glm_mat4_mul(camera_local_mat_position, main_camera.view, tranlate_dot_rot);

}

void move_player_forward(){
    //LOG("Action pointer work\n");
    update_translation(VEC3(0,-0.3,0));

}

void move_player_backward(){
    update_translation(VEC3(0,0.3,0));
}

void rotate_player_left(){
     versor new_rot_quat;
    glm_quatv(new_rot_quat, glm_rad(5), VEC3(0,0,1) );
    rotate_element(selected_element,new_rot_quat);
}

void rotate_player_right(){
       versor new_rot_quat;
    glm_quatv(new_rot_quat, glm_rad(-5), VEC3(0,0,1) );
    rotate_element(selected_element,new_rot_quat);
}

void init_game(){
      
    add_action_function(&move_player_forward);
    add_action_function(&move_player_backward);
    add_action_function(&rotate_player_left);
    add_action_function(&rotate_player_right);

    game_initialized = true;
}


void update_player_camera(){
    mat4 inverse_player_matrix;
    glm_mat4_inv(player1->transform->model_matrix, inverse_player_matrix);

    glm_mat4_mul(tranlate_dot_rot,inverse_player_matrix,main_camera.view);

}

void update_game(){
    update_player_camera(); 
}