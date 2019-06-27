#include "input.h"

#include "../utils.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "../engine.h"

#include <cglm/cglm.h>
#include "../camera.h"

#include <math.h>

#include "editor.h"

#include "text.h"

bool move_camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    Key* actual_key = NULL;
    switch (key)
    {
    case GLFW_KEY_ENTER:
        actual_key = &input.ENTER;
        break;
    case GLFW_KEY_Z:
        actual_key = &input.Z;
        break;
    case GLFW_KEY_A:
        actual_key = &input.A;
        break;
    case GLFW_KEY_D:
        actual_key = &input.D;
        break;
    case GLFW_KEY_W:
        actual_key = &input.W;
        break;
    case GLFW_KEY_S:
        actual_key = &input.S;
        break;
    case GLFW_KEY_G:
        actual_key = &input.G;
        break;
    case GLFW_KEY_J:
        actual_key = &input.J;
        break;
    case GLFW_KEY_K:
        actual_key = &input.K;
        break;
    case GLFW_KEY_X:
        actual_key = &input.X;
        break;
    case GLFW_KEY_Q:
        actual_key = &input.Q;
        break;
    case GLFW_KEY_E:
        actual_key = &input.E;
        break;
    case GLFW_KEY_T:
        actual_key = &input.T;
        break;
    case GLFW_KEY_L:
        actual_key = &input.L;
        break;
     case GLFW_KEY_ESCAPE:
        actual_key = &input.ESC;
        break;
    default:
        break;
    }

    if(actual_key != NULL){
        if(action == GLFW_PRESS){
            actual_key->bIsPressed = true;
            actual_key->Released = false;
        }
        if(action == GLFW_RELEASE){
            actual_key->bIsPressed = false;
            actual_key->Released = true;
        }
    } 
  
           
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
                move_camera = false;
				
			}
			if(action == GLFW_RELEASE){
			
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                move_camera = true;
			
			}
			if(action == GLFW_RELEASE){
				
			}

		}

}

static inline bool key_released(Key* key){
    if(key->Released){
        key->Released = false;
        return true;
    }
    return false;
}

void init_input(){
    memset(&input,0,sizeof(Input));
}

void grab_mode(){
    if(key_released(&input.G)){
        editor_mode = DEFAULT_MODE;
        editor_mode_show_text = "Default Mode";
        return;
    }
    editor_mode_show_text = "Grab Mode";
        
    if(input.W.bIsPressed){
        vec3 move = {0,-0.02,0};
    glm_translate(selected_element->model->model_mat, move);
    glm_vec3_add(selected_element->position,move,selected_element->position);
    }
    if(input.S.bIsPressed){
        vec3 move = {0,0.02,0};
    glm_translate(selected_element->model->model_mat, move);
    glm_vec3_add(selected_element->position,move,selected_element->position);
    }
    if(input.D.bIsPressed){
            vec3 move = {-0.02,0,0};
    glm_translate(selected_element->model->model_mat, move);
    glm_vec3_add(selected_element->position,move,selected_element->position);
    }
    if(input.A.bIsPressed){
        vec3 move = {0.02,0,0};
    glm_translate(selected_element->model->model_mat, move);
    glm_vec3_add(selected_element->position,move,selected_element->position);

    }
}

void navigate_mode(){
    if(move_camera){
        if(input.W.bIsPressed){
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04},camera_front,move);
            glm_vec3_add(camera_position,move,camera_position);
            update_look_at();
        }
        if(input.S.bIsPressed){
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04},camera_front,move);
            glm_vec3_sub(camera_position,move,camera_position);
            update_look_at();
        }
        if(input.D.bIsPressed){
            vec3 cross;
            glm_vec3_cross(camera_front, camera_up, cross);
            glm_normalize(cross);
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04}, cross, move );
            glm_vec3_add(camera_position, move,camera_position);
            update_look_at();
        }
        if(input.A.bIsPressed){
            vec3 cross;
            glm_vec3_cross(camera_front, camera_up, cross);
            glm_normalize(cross);
            vec3 move;
            glm_vec3_mul((vec3){0.04,0.04,0.04}, cross, move );
            glm_vec3_sub(camera_position, move,camera_position);
            update_look_at();
        }
    }
}

void default_mode(){
    if(!move_camera){
        if(key_released(&input.S)){
            get_element_status(selected_element);
        }
        
        if(input.X.bIsPressed){
            save_data();
        }
        if(key_released(&input.Z)){
            load_level_in_editor();            
        }
        if(key_released(&input.A)){
            add_element_menu.show = true;
            add_element_menu.type = MENU_TYPE_ADD_MODEL;
        }

        
        if(key_released(&input.G)){
            editor_mode = GRAB_MODE;
        }

        if(key_released(&input.Q)){
            get_elements_in_editor_map();
        }
       
        if(key_released(&input.T)){
            add_element_menu.show = true;
            add_element_menu.type = MENU_TYPE_ADD_TEXTURE;
        }
        

        if(add_element_menu.show){
            if(key_released(&input.J)){
                mark_id++;
            }
            if(key_released(&input.K)){
                mark_id--;
            }
            if(key_released(&input.ENTER)){
                if(add_element_menu.type == MENU_TYPE_ADD_MODEL)
                    open_file = 5;
                if(add_element_menu.type == MENU_TYPE_ADD_TEXTURE)
                    add_texture = true;
                    
                add_element_menu.element_selected = true;
            }
        }

    }
}
void update_input(){
    switch (editor_mode)
    {    
    case DEFAULT_MODE:
        default_mode();
        break;
    case NAVIGATE_MODE:
        navigate_mode();
    case GRAB_MODE:
        grab_mode();
        break;

    default:
        break;
    } 
}

float last_mouse_x = 400;
float last_mouse_y = 300;
bool first_mouse_movement = true;



void camera_mouse_control(float yaw, float pitch){
    vec3 front;
    
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    

    glm_normalize(front);

    glm_vec3_copy(front, camera_front);

    update_look_at();
}
float horizontalAngle = 0;
float verticalAngle = 0;
void mouse_movement_control(float xpos, float ypos){
    
  
    
    horizontalAngle += 800/2 - xpos ;
    
    verticalAngle  += 600/2 - ypos ;

    horizontalAngle *= 0.05;
    verticalAngle *= 0.05;

    camera_mouse_control(0, horizontalAngle);
    

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(move_camera)
        mouse_movement_control(xpos, ypos);
}