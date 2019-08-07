//
// Created by pavon on 6/24/19.
//

#include "game.h"
#include "input.h"

#include "gui.h"
#include "third_party/cglm/cglm.h"

#include "camera.h"

#include "engine.h"


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

void update_game(){
    selected_element = get_from_array(actual_elements_array,0);
    CameraComponent* camera = get_component_from_element(selected_element,CAMERA_COMPONENT);
    update_main_camera_with_camera_component_values(camera);
}