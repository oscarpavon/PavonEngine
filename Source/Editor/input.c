#include "input.h"

#include "../Engine/utils.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "../Engine/engine.h"

#include "../ThirdParty/cglm/cglm.h"
#include "../Engine/camera.h"

#include <math.h>

#include "editor.h"

#include "text.h"

#include "../Engine/game.h"

#include "editor_mode.h"
#include "../Engine/gui.h"

#include "commands.h"

float rotate_value = 100;

float horizontalAngle = 0;
float verticalAngle = 0;


float last_mouse_x = 400;
float last_mouse_y = 300;
bool first_mouse_movement = true;


bool left_click = false;
float actual_mouse_position_x;
float actual_mouse_position_y;

//grab mode
float move_object_value = 0.5;
bool grid_translate = false;
vec2 move_ui_element_value;//per pixel
float move_ui_element_value_per_axis = 0.6;

bool player_in_start_position = false;


void mouse_movement_control(float xpos, float ypos){   

    horizontalAngle += camera_width_screen/2 - xpos ;
    
    verticalAngle  += camera_heigth_screen/2 - ypos ;

    horizontalAngle *= 0.05;
    verticalAngle *= 0.05;

    camera_rotate_control(0, horizontalAngle);   
     
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(mouse_navigate_control)
        mouse_movement_control(xpos, ypos);

    actual_mouse_position_x = xpos;
    actual_mouse_position_y = ypos;
         
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    Key* actual_key = NULL;
    Key* actual_mod_key = NULL;
    switch (key)
    {    
    case GLFW_KEY_LEFT_SHIFT:
        actual_mod_key = &input.SHIFT;
        break;
    case GLFW_KEY_LEFT_ALT:
        actual_mod_key = &input.ALT;
        break;
    case GLFW_KEY_BACKSPACE:
        actual_key = &input.BACKSPACE;
        break;
    case GLFW_KEY_ENTER:
        actual_key = &input.ENTER;
        break;
    case GLFW_KEY_A:
        actual_key = &input.A;
        break;
    case GLFW_KEY_B:
        actual_key = &input.B;
        break;
    case GLFW_KEY_C:
        actual_key = &input.C;
        break;
    case GLFW_KEY_D:
        actual_key = &input.D;
        break;
    case GLFW_KEY_E:
        actual_key = &input.E;
        break;
    case GLFW_KEY_F:
        actual_key = &input.F;
        break;
    case GLFW_KEY_G:
        actual_key = &input.G;
        break;
    case GLFW_KEY_H:
        actual_key = &input.H;
        break;
    case GLFW_KEY_I:
        actual_key = &input.I;
        break;
    case GLFW_KEY_J:
        actual_key = &input.J;
        break;
    case GLFW_KEY_K:
        actual_key = &input.K;
        break;
    case GLFW_KEY_L:
        actual_key = &input.L;
        break;
    case GLFW_KEY_M:
        actual_key = &input.M;
        break;
    case GLFW_KEY_N:
        actual_key = &input.N;
        break;
    case GLFW_KEY_O://letter "o"
        actual_key = &input.O;
        break;
    case GLFW_KEY_P:
        actual_key = &input.P;
        break;
    case GLFW_KEY_Q:
        actual_key = &input.Q;
        break;
    case GLFW_KEY_R:
        actual_key = &input.R;
        break;
    case GLFW_KEY_S:
        actual_key = &input.S;
        break;
    case GLFW_KEY_T:
        actual_key = &input.T;
        break;
    case GLFW_KEY_V:
        actual_key = &input.V;
        break;
    case GLFW_KEY_Y:
        actual_key = &input.Y;
        break;
    case GLFW_KEY_W:
        actual_key = &input.W;
        break;
    case GLFW_KEY_X:
        actual_key = &input.X;
        break;
    case GLFW_KEY_Z:
        actual_key = &input.Z;
        break;
    case GLFW_KEY_0://number zero
        actual_key = &input.KEY_0;
        break;
    case GLFW_KEY_1:
        actual_key = &input.KEY_1;
        break;    
    case GLFW_KEY_2:
        actual_key = &input.KEY_2;
        break;
    case GLFW_KEY_3:
        actual_key = &input.KEY_3;
        break;
    case GLFW_KEY_4:
        actual_key = &input.KEY_4;
        break;
    case GLFW_KEY_5:
        actual_key = &input.KEY_5;
        break;
    case GLFW_KEY_6:
        actual_key = &input.KEY_6;
        break;
    case GLFW_KEY_7:
        actual_key = &input.KEY_7;
        break;
    case GLFW_KEY_8:
        actual_key = &input.KEY_8;
        break;
    case GLFW_KEY_9:
        actual_key = &input.KEY_9;
        break;
    case GLFW_KEY_DOWN:
        actual_key = &input.KEY_DOWN;
        break;
    case GLFW_KEY_UP:
        actual_key = &input.KEY_UP;
        break;
    case GLFW_KEY_TAB:
        actual_key = &input.TAB;
        break;
    case GLFW_KEY_ESCAPE:
        actual_key = &input.ESC;
        break;
    
    default:
        break;
    }

    if(actual_key != NULL){
        if(action == GLFW_PRESS){
            actual_key->pressed = true;
            actual_key->Released = false;
            actual_key->mods = mods;
        }
        if(action == GLFW_RELEASE){
            actual_key->pressed = false;
            actual_key->Released = true;
            
        }
    } 
  
    if(actual_mod_key != NULL){
        if(action == GLFW_PRESS){
            actual_mod_key->pressed = true;
            actual_mod_key->Released = false;
        }
        if(action == GLFW_RELEASE){
            actual_mod_key->pressed = false;
            actual_mod_key->Released = true;
            
        }
    } 
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

		if (button ==  GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
                move_camera_input = false;
                mouse_navigate_control = false;
                //change_to_editor_mode(EDITOR_DEFAULT_MODE);
				left_click = true;
                touch_position_x = actual_mouse_position_x;
                touch_position_y = actual_mouse_position_y;
			}
			if(action == GLFW_RELEASE){
                left_click = false;
                touch_position_x = -1;
                touch_position_y = -1;
			}

		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                move_camera_input = true;
                mouse_navigate_control = true;
                change_to_editor_mode(EDITOR_NAVIGATE_MODE);
			
			}
			if(action == GLFW_RELEASE){
				
			}

		}

}


void init_input(){
    memset(&input,0,sizeof(Input));
}

void input_change_mode(){
  if(selected_element != NULL){ 
	if(editor_mode != EDITOR_NAVIGATE_MODE){
		if(key_released(&input.S)){
			 change_to_editor_sub_mode(EDITOR_SUB_MODE_SCALE);
		}
	}

    if(key_released(&input.G)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_GRAB);
    }

    if(key_released(&input.R)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_ROTATE);
    }
  }
  if(key_released(&input.V)){
        change_to_editor_mode(EDITOR_NAVIGATE_MODE);
  }
}

bool editor_input_camera_rotate_control(){
   bool rotate_keys_pressed = false;
	if(input.J.pressed){
		rotate_keys_pressed = true;
        rotate_value -= 10;
        if(rotate_value < -10000){
            rotate_value = -100;
        }

    }

    if(input.K.pressed){
		rotate_keys_pressed = true;
        rotate_value += 10;
        if(rotate_value > 10000){
            rotate_value = 100;
        }

    }
	if(rotate_keys_pressed){
		
        horizontalAngle += camera_width_screen/2 - rotate_value ;
        
		verticalAngle  += 600/2 - 100 ;

        horizontalAngle *= 0.05;
        verticalAngle *= 0.05;

        camera_rotate_control(0, horizontalAngle);
		return true;
	}
	return false;
}


void editor_input_navigate(){
    input_change_mode();

    if(input.I.pressed){
        camera_velocity += 0.02;
    }
    if(input.O.pressed){
        camera_velocity -= 0.02;
    }


    vec3 velocity_vector;
    float velocity_per_frame = camera_velocity * time_delta;
    glm_vec3_copy((vec3){velocity_per_frame,velocity_per_frame,velocity_per_frame}, velocity_vector);
    bool update = false;

    if(input.E.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.up,move);
        glm_vec3_add(main_camera.position,move,main_camera.position);
        update = true;
    }
    if(input.Q.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.up,move);
        glm_vec3_sub(main_camera.position,move,main_camera.position);
        update = true;
    }


    if(input.W.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.front,move);
        glm_vec3_add(main_camera.position,move,main_camera.position);
        update = true;
    }
    if(input.S.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.front,move);
        glm_vec3_sub(main_camera.position,move,main_camera.position);
        update = true;
    }
    if(input.D.pressed){
        vec3 cross;
        glm_vec3_cross(main_camera.front, main_camera.up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_add(main_camera.position, move,main_camera.position);
        update = true;
    }
    if(input.A.pressed){
        vec3 cross;
        glm_vec3_cross(main_camera.front, main_camera.up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_sub(main_camera.position, move,main_camera.position);
        update = true;
    }
    
    if(editor_input_camera_rotate_control()){	
		//camera_update(&current_window->camera);
        camera_update(&main_camera);
	} 
	if(update)
        camera_update(&main_camera);
}


void grab_mode(){
    draw_translate_gizmo = true;
    draw_rotate_gizmo = false;
    gizmos_draw_scale = false;

    if(key_released(&input.KEY_1)){
        grid_translate = true;
    }
    if(!grid_translate){
        if(key_released(&input.I)){
            if(editor_mode == EDITOR_DEFAULT_MODE)
                move_object_value += 0.00005;

            if(editor_mode == EDITOR_MODE_GUI_EDITOR)
                move_ui_element_value_per_axis += 0.1;
        }
        if(key_released(&input.O)){
            if(editor_mode == EDITOR_DEFAULT_MODE)
                move_object_value -= 0.00005;

            if(editor_mode == EDITOR_MODE_GUI_EDITOR)
                move_ui_element_value_per_axis -= 0.1;
        }
    }
    
    input_change_mode();
        

    if(editor_mode == EDITOR_DEFAULT_MODE){
        editor_input_camera_rotate_control();     
        if(input.SHIFT.pressed){
            editor_input_navigate();
        }
        vec3 move;
        bool update = false;
        bool camera_space = true;
        if(!grid_translate){
            if(camera_space){
                vec3 velocity_vector;
                glm_vec3_copy(VEC3(move_object_value,move_object_value,move_object_value) , velocity_vector);
                if(input.D.pressed){                    
                    vec3 cross;
                    glm_vec3_crossn(main_camera.front, main_camera.up, cross);                    
                    glm_vec3_mul(velocity_vector, cross , move );
                    update = true;
                }else if(input.A.pressed){
                    vec3 cross;
                    glm_vec3_crossn(main_camera.front, main_camera.up, cross);
                    glm_vec3_inv(velocity_vector); 
                    glm_vec3_mul(velocity_vector, cross , move );
                    update = true;
                }else if(input.W.pressed){
                    glm_vec3_mul(velocity_vector,main_camera.front,move);
                    update = true;
                }else if(input.S.pressed){
                    glm_vec3_inv(velocity_vector);
                    glm_vec3_mul(velocity_vector,main_camera.front,move);
                    update = true;
                }else if(input.E.pressed){
                    glm_vec3_mul(velocity_vector,main_camera.up,move);
                    update = true;
                }else if(input.Q.pressed){
                    glm_vec3_inv(velocity_vector);
                    glm_vec3_mul(velocity_vector,main_camera.up,move);
                    update = true;
                }                                
            }else{//World Transform
                if(input.W.pressed){
                    glm_vec3_copy( VEC3(0,-move_object_value,0) , move );
                    update = true;
                }
                if(input.S.pressed){
                    glm_vec3_copy( VEC3(0,move_object_value,0) , move );
                    update = true;            }
                if(input.D.pressed){
                    glm_vec3_copy( VEC3(-move_object_value,0,0) , move );                
                    update = true;            
                }
                if(input.A.pressed){
                    glm_vec3_copy( VEC3(move_object_value,0,0) , move );
                    update = true;            
                }
                if(input.E.pressed){
                    glm_vec3_copy( VEC3(0,0,move_object_value) , move );
                    update = true;            
                }
                if(input.Q.pressed){
                    glm_vec3_copy( VEC3(0,0,-move_object_value) , move );
                    update = true;            
                }
            }           
            
            if(current_component_selected != NULL){
                if(update){
                    CameraComponent* camera = current_component_selected->data;
                    glm_vec3_add(camera->position,move,camera->position);
                }              
            }else
            {
                if(update)
                    update_translation(move);
            }            
            
            
        }else{//grid translate
            if(key_released(&input.KEY_1)){
                grid_translate = false;
            } 
            
            if(key_released(&input.D)){
                vec3 move = {move_object_value,0,0};
                update_translation(move);
            }
            if(key_released(&input.A)){
                vec3 move = {-move_object_value,0,0};
                update_translation(move);
            }
            if(key_released(&input.S)){
                vec3 move = {0,-move_object_value,0};
                update_translation(move);
            }
            if(key_released(&input.W)){
                vec3 move = {0,move_object_value,0};
                update_translation(move);
            }
            if(key_released(&input.I)){
                move_object_value += 2.4;
            }
            if(key_released(&input.O)){
                move_object_value = 24;
            }
        }
    }

    if(editor_mode == EDITOR_MODE_GUI_EDITOR){
        if(selected_button == NULL)
            return;
        if(input.W.pressed){
            move_ui_element_value[0] = 0;
            move_ui_element_value[1] = move_ui_element_value_per_axis;
            glm_vec3_sub(selected_button->position, move_ui_element_value, selected_button->position);
        }
        if(input.S.pressed){
            move_ui_element_value[0] = 0;
            move_ui_element_value[1] = move_ui_element_value_per_axis;            
            glm_vec3_add(selected_button->position, move_ui_element_value, selected_button->position);
        }
        if(input.D.pressed){
            move_ui_element_value[1] = 0;
            move_ui_element_value[0] = move_ui_element_value_per_axis;             
            glm_vec3_add(selected_button->position, move_ui_element_value, selected_button->position);
        }
        if(input.A.pressed){
            move_ui_element_value[1] = 0;
            move_ui_element_value[0] = move_ui_element_value_per_axis;            
            glm_vec3_sub(selected_button->position, move_ui_element_value, selected_button->position);
        }
    }
    
}




void scale_mode(){
    
    input_change_mode();

    gizmos_draw_scale = true;

    if(editor_mode == EDITOR_DEFAULT_MODE){     
        
        vec3 move;
        bool update = false;
                  
            if(input.J.pressed){
                glm_vec3_copy( VEC3(0,-move_object_value,0) , move );
                update = true;
            }
            if(input.K.pressed){
                glm_vec3_copy( VEC3(0,move_object_value,0) , move );
                update = true;            }
            if(input.D.pressed){
                glm_vec3_copy( VEC3(-move_object_value,0,0) , move );
                update = true;            
            }
            if(input.A.pressed){
                glm_vec3_copy( VEC3(move_object_value,0,0) , move );
                update = true;            
            }
            if(input.E.pressed){
                glm_vec3_copy( VEC3(0,0,move_object_value) , move );
                update = true;            
            }
            if(input.Q.pressed){
                glm_vec3_copy( VEC3(0,0,-move_object_value) , move );
                update = true;            
            }
            
            if(update)
                update_scale(move);
            
        }
}

void default_mode(){
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
        return;
        
    draw_rotate_gizmo = false;
    draw_translate_gizmo = false;
    gizmos_draw_scale = false;

    input_change_mode();
        
    //edit in blender
    if(key_released(&input.TAB)){
        if(selected_element){
            if(selected_element->editor_data.has_blend_file){
                char relative_path[] = "../assets/";
                char new_file_path[strlen(relative_path) + strlen(selected_element->editor_data.blend_file_path) ];
                memset(new_file_path,0,strlen(new_file_path));
                strcat(new_file_path,relative_path);
                strcat(new_file_path,selected_element->editor_data.blend_file_path);
                system_command("blender ", new_file_path);
                strcpy(editing_blender_file_path,new_file_path);
                is_editing_blender_file = true;
            }
        }
    }

    if(key__released(&input.D,GLFW_MOD_SHIFT)){
       duplicate_selected_element(1,selected_element);
       LOG("duplicated \n");
       return; 
    }
    if(key__released(&input.A,GLFW_MOD_ALT)){
        LOG("deselect all \n");
        deselect_all();
        return;  
    }
    
    if(key__released(&input.P,GLFW_MOD_SHIFT)){
        LOG("editor play mode \n");
        change_to_editor_mode(EDITOR_PLAY_MODE);
        return; 
    }

    if(key__released(&input.P,GLFW_MOD_CONTROL)){
        //TODO: open game in new window
        LOG("playing \n");
        play_game_standalone();
        return; 
    }

    if(key_released(&input.X)){
        remove_selected_element();            
        return; 
    }   

    if(key_released(&input.F)){
        editor_focus_selected_element();
    }

    if(key_released(&input.KEY_1)){
        if(can_draw_gizmos){
            can_draw_gizmos = false;
        }else can_draw_gizmos = true;
        return;
    }
    if(key_released(&input.KEY_2)){
        init_skeletal_editor();
        can_draw_skeletal_bones = true;
        return; 
    }
    if(key_released(&input.KEY_3)){        
        can_draw_bounding_box_in_select_element = true;
        return; 
    }
           
    if(key_released(&input.Q)){
        if(controlling_camera_component){
            controlling_camera_component = false;
            memcpy(&main_camera,&saved_camera, sizeof(CameraComponent));
            camera_update(&main_camera);
            return;
        }
        CameraComponent* camera_component = get_component_from_selected_element(CAMERA_COMPONENT);
        if(camera_component){
            controlling_camera_component = true;
            change_view_to_camera_component(camera_component);
        }
    }
	if(key_released(&input.M)){
		editor_file_explorer_show = true;
	}
	if(key_released(&input.A)){
		editor_content_browser_show = true;
	}
}

void rotate_input_mode(){
    input_change_mode();

    draw_rotate_gizmo = true;
    draw_translate_gizmo = false;
    gizmos_draw_scale = false;

    if(key_released(&input.R)){
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        return;
    }

    if(key_released(&input.X)){

    }

    if(input.SHIFT.pressed){
        if(key_released(&input.J)){
            rotate_editor_element(selected_element, -5, (vec3){0,0,1});
        }
        if(key_released(&input.K)){
            
            rotate_editor_element(selected_element, 5, (vec3){0,0,1});
        }
    }
    else{
        float rotate_value = 1;
        if(input.J.pressed){
            rotate_editor_element(selected_element, -rotate_value, (vec3){0,0,1});
        }
        if(input.K.pressed){
            
            rotate_editor_element(selected_element, rotate_value, (vec3){0,0,1});
        }
    }
}


void input_mode_play(){
  
    //update_game();
    if(loop_fuction_dynamic_loaded)
        loop_fuction_dynamic_loaded(&input);

    controlling_camera_component = true;

    if(key_released(&input.ESC)){

        player_in_start_position = false;
        controlling_camera_component = false;
        loaded_gamplay_library = false;
        loop_fuction_dynamic_loaded = NULL;
        close_dynamic_game_play();
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        return;
    }
    if(!player_in_start_position){
        if(player1 != NULL){
            if(player_start == NULL){
                //LOG("No player start\n");
                //return;
            }            

            //set_element_position(player1,player_start->transform->position);
            player_in_start_position = true;
        }else{
            //LOG("No player selected\n");
           // return;
        }
    }
    
}

void input_gui_editor(){
    if(editor_sub_mode != EDITOR_SUB_MODE_NULL)
        return;
    
    if(key_released(&input.ESC)){
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        //array_clean(actual_buttons_array);
    }
        
        
    if(key__released(&input.A,GLFW_MOD_SHIFT)){
        new_empty_button();
    }

    if(key_released(&input.G)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_GRAB);
    }
}

void editor_window_level_editor_input_update(){
   //Editor Normal Mode 
	if(editor_sub_mode == EDITOR_SUB_MODE_NULL){
        switch (editor_mode)
        {    
        case EDITOR_DEFAULT_MODE:
            default_mode();
            break;
        case EDITOR_NAVIGATE_MODE:
            editor_input_navigate();
            break;        
        case EDITOR_PLAY_MODE:
            input_mode_play();
            break;
        case EDITOR_MODE_GUI_EDITOR:
            input_gui_editor();
            break;
        case EDITOR_CHANGING_MODE_MODE:
            editor_mode = mode_to_change;
            break;
        default:
            break;
        } 

    }
   //Editor Sub Mode 
    switch (editor_sub_mode)
    {
    case EDITOR_SUB_MODE_TEXT_INPUT:
        text_input_mode();
        break;
    case EDITOR_SUB_MODE_GRAB:
        grab_mode();            
        break;
    case EDITOR_SUB_MODE_SCALE:
        scale_mode();
        break;
    case EDITOR_SUB_MODE_ROTATE:
        rotate_input_mode();
        break;
    case EDITOR_SUB_MODE_NULL:
        break;

    default:
        break;
    }
   
}
