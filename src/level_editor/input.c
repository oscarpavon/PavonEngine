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

#include "../game.h"

#include "editor_mode.h"

#include "../gui.h"

#include "../input.h"

float horizontalAngle = 0;
float verticalAngle = 0;

static size_t code_to_utf8(unsigned char *const buffer, const unsigned int code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}


bool activate_text_input_mode = false;
unsigned short int character_count = 0;

char first_char_command;
char* command_array_pointer;
char* argument_array_pointer;

void parse_command(const char* command){
    first_char_command = command[1];

    if(strcmp(&command[1],"gui") == 0){
        change_to_editor_mode(EDITOR_MODE_GUI_EDITOR);
    }
    
    if(editor_mode == EDITOR_MODE_GUI_EDITOR){
         if(command[1] == 'w'){
            
            save_gui_data(&command[3]);
            LOG("GUI saved: %s\n",&command[3]);
        }
        if(command[1] == 'o'){
            load_gui(&command[3]);       
            LOG("GUI loaded\n");
        }
        if(first_char_command == 's'){
            if(command[2] == 'n'){
                strcpy(selected_button->name,&command[4]);
            }
        }
        

    }else{
        if(command[1] == 'w'){
            save_level_data(&command[3]);
            LOG("Level saved: %s\n",&command[3]);
        }
        if(command[1] == 'o'){        
            load_level_in_editor(&command[3]);
            LOG("Level loaded\n");
            return;
        }
    }
    
    switch (first_char_command)
    {
    case 'r':{
        reload_editor();
        LOG("reload\n");
    }
    break;
    
    default:
        break;
    }
    if(command[1] == 'q'){
        exit(0);
    }
    if(command[1] == 'r'){        
       
    }
    if(command[1] == 's'){ //set
        if(command[3] == 'p'){//player
            player1 = selected_element;            
            add_editor_native_element("Player Controller");
        }
    }
    if(command[1] == 'h'){        
       /*  selected_element->has_HLOD = true;
        LOG("HLOD added\n"); */
    }
    if(command[1] == 'd'){ 
        unsigned int duplicate_count = atoi(&command[3]); 
        for(int i = 0; i< duplicate_count; i++){
            duplicate_selected_element();        
            vec3 move = {24,0,0};
        }      
        LOG("duplicated %i\n",duplicate_count);
    }
    if(command[1] == 'h'){
        
    }
}

void parse_characters(unsigned char character){
    if(character == ':'){
        LOG("Command mode\n");
        command_text_buffer[character_count] = character;
        character_count++;        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_TEXT_INPUT);
        return;
    }
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT){
        command_text_buffer[character_count] = character;
        character_count++;
    }
}

void text_input_mode(){
    if(key_released(&input.ENTER)){
        parse_command(command_text_buffer);
        character_count = 0;
        memset(command_text_buffer,0,sizeof(command_text_buffer));
        
        change_to_editor_sub_mode(EDITOR_SUB_MODE_NULL);
    }
    if(key_released(&input.BACKSPACE)){
        character_count--;
        command_text_buffer[character_count] = '\0';
    }

}

void character_callback(GLFWwindow* window, unsigned int codepoint){
    if(codepoint == 241)//equal "ñ"
            return;
    unsigned char character[1];
    code_to_utf8(character,codepoint);
    //LOG("Converted: %s\n",character);
    parse_characters(character[0]);
}

bool first_camera_rotate = true;
vec3 init_front;
void camera_rotate_control(float yaw, float pitch){
    vec3 front;

    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    if(first_camera_rotate == true){
        glm_vec3_copy(main_camera.front,init_front);
        glm_vec3_mul(init_front,(vec3){0,5,0},init_front);
        first_camera_rotate = false;
    }
    //glm_vec3_add(init_front,front,front);

    //glm_vec3_rotate(front,45,(vec3){0,0,1});    
    glm_normalize(front);

    glm_vec3_copy(front, main_camera.front);

    update_look_at();
}
bool left_click = false;
float actual_mouse_position_x;
float actual_mouse_position_y;

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
    case GLFW_KEY_R:
        actual_key = &input.R;
        break;
    case GLFW_KEY_1:
        actual_key = &input.KEY_1;
        break;    
    case GLFW_KEY_ESCAPE:
        actual_key = &input.ESC;
        break;
    case GLFW_KEY_V:
        actual_key = &input.V;
        break;
    case GLFW_KEY_P:
        actual_key = &input.P;
        break;
    case GLFW_KEY_O://letter "o"
        actual_key = &input.O;
        break;
    case GLFW_KEY_I://letter "o"
        actual_key = &input.I;
        break;
    case GLFW_KEY_2:
        actual_key = &input.KEY_2;
        break;
    case GLFW_KEY_3:
        actual_key = &input.KEY_3;
        break;
    case GLFW_KEY_0://number zero
        actual_key = &input.KEY_0;
        break;
    case GLFW_KEY_9:
        actual_key = &input.KEY_9;
        break;
    case GLFW_KEY_C:
        actual_key = &input.C;
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
    if(key_released(&input.G)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_GRAB);
    }

    if(key_released(&input.R)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_ROTATE);
    }
    if(key_released(&input.V)){
        change_to_editor_mode(EDITOR_NAVIGATE_MODE);
    }
}


float move_object_value = 0.02;
bool grid_translate = false;
vec2 move_ui_element_value;//per pixel
float move_ui_element_value_per_axis = 0.6;
void grab_mode(){
    draw_translate_gizmo = true;
    draw_rotate_gizmo = false;

    if(key_released(&input.KEY_1)){
        grid_translate = true;
    }
    if(!grid_translate){
        if(key_released(&input.I)){
            if(editor_mode == EDITOR_DEFAULT_MODE)
                move_object_value += 0.04;

            if(editor_mode == EDITOR_MODE_GUI_EDITOR)
                move_ui_element_value_per_axis += 0.1;
        }
        if(key_released(&input.O)){
            if(editor_mode == EDITOR_DEFAULT_MODE)
                move_object_value -= 0.04;

            if(editor_mode == EDITOR_MODE_GUI_EDITOR)
                move_ui_element_value_per_axis -= 0.1;
        }
    }
    input_change_mode();

    if(editor_mode == EDITOR_DEFAULT_MODE){     

        if(!grid_translate){            
            if(input.W.pressed){
                vec3 move = {0,-move_object_value,0};
                update_translation(move);
            }
            if(input.S.pressed){
                vec3 move = {0,move_object_value,0};
                update_translation(move);
            }
            if(input.D.pressed){
                vec3 move = {-move_object_value,0,0};
                update_translation(move);
            }
            if(input.A.pressed){
                vec3 move = {move_object_value,0,0};
                update_translation(move);
            }
            if(input.E.pressed){
                vec3 move = {0,0,move_object_value};
                update_translation(move);
            }
            if(input.Q.pressed){
                vec3 move = {0,0,-move_object_value};
                update_translation(move);
            }
            
        }else{
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

float rotate_value = 100;

void navigate_mode(){
    input_change_mode();

    if(input.I.pressed){
        camera_velocity += 0.02;
    }
    if(input.O.pressed){
        camera_velocity -= 0.02;
    }


    vec3 velocity_vector;
    glm_vec3_copy((vec3){camera_velocity,camera_velocity,camera_velocity}, velocity_vector);

    if(input.E.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.up,move);
        glm_vec3_add(main_camera.position,move,main_camera.position);
        update_look_at();
    }
    if(input.Q.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.up,move);
        glm_vec3_sub(main_camera.position,move,main_camera.position);
        update_look_at();
    }
    if(input.J.pressed){
        horizontalAngle += camera_width_screen/2 - rotate_value ;
        rotate_value -= 30;
        if(rotate_value < -10000){
            rotate_value = -100;
        }

        verticalAngle  += 600/2 - 100 ;

        horizontalAngle *= 0.05;
        verticalAngle *= 0.05;
        
        camera_rotate_control(0, horizontalAngle);
    }
    if(input.K.pressed){
         horizontalAngle += camera_width_screen/2 - rotate_value ;
        rotate_value += 30;
        if(rotate_value > 10000){
            rotate_value = 100;
        }

        verticalAngle  += 600/2 - 100 ;

        horizontalAngle *= 0.05;
        verticalAngle *= 0.05;

        camera_rotate_control(0, horizontalAngle);
    }

    if(input.W.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.front,move);
        glm_vec3_add(main_camera.position,move,main_camera.position);
        update_look_at();
    }
    if(input.S.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,main_camera.front,move);
        glm_vec3_sub(main_camera.position,move,main_camera.position);
        update_look_at();
    }
    if(input.D.pressed){
        vec3 cross;
        glm_vec3_cross(main_camera.front, main_camera.up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_add(main_camera.position, move,main_camera.position);
        update_look_at();
    }
    if(input.A.pressed){
        vec3 cross;
        glm_vec3_cross(main_camera.front, main_camera.up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_sub(main_camera.position, move,main_camera.position);
        update_look_at();
    }
    
}
bool controlling_camera_component = false;
void default_mode(){
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
        return;
        
    draw_rotate_gizmo = false;
    draw_translate_gizmo = false;

    input_change_mode();

    if(key__released(&input.D,GLFW_MOD_ALT)){
       duplicate_selected_element();
       LOG("duplicated \n"); 
    }
    if(key__released(&input.A,GLFW_MOD_ALT)){
        LOG("deselect all \n");
        deselect_all(); 
    }
    if(key_released(&input.S)){
        //get_element_status(selected_element);
    }
    
    if(key__released(&input.P,GLFW_MOD_SHIFT)){
        LOG("editor play mode \n");
        change_to_editor_mode(EDITOR_PLAY_MODE);
    }

    if(key__released(&input.P,GLFW_MOD_CONTROL)){
        //TODO: open game in new window
        LOG("playing \n");
        play_game_standalone();
    }
  
    if(key_released(&input.X)){
        remove_selected_element();            
    } 
    
    
    if(key_released(&input.P)){
       
    }

    if(key_released(&input.KEY_1)){
        if(can_draw_gizmos){
            can_draw_gizmos = false;
        }else can_draw_gizmos = true;

    }
    if(key_released(&input.KEY_2)){
        init_skeletal_editor();
        can_draw_skeletal_bones = true;
    }
           
    can_open_text_menu_with_key(&add_element_menu,&input.A,GLFW_MOD_SHIFT);
    can_open_text_menu_with_key(&menu_editor_element_list,&input.L,NULL);
    can_open_text_menu_with_key(&menu_add_texture,&input.T,GLFW_MOD_SHIFT); 

    
    can_open_text_menu_with_key(&menu_add_native_editor_element, &input.E,GLFW_MOD_SHIFT);

    if(key_released(&input.Q)){
        if(controlling_camera_component){
            controlling_camera_component = false;
            memcpy(&main_camera,&saved_camera, sizeof(CameraComponent));
            update_look_at();
            return;
        }
        CameraComponent* camera_component = get_component_from_selected_element(CAMERA_COMPONENT);
        if(camera_component){
            controlling_camera_component = true;
            change_view_to_camera_component(camera_component);
        }
    }
}

void rotate_input_mode(){
    input_change_mode();

    draw_rotate_gizmo = true;
    draw_translate_gizmo = false;

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

bool player_in_start_position = false;

void input_mode_play(){
  
    update_game();

    if(input.W.pressed){
        vec3 move = {0,-move_object_value,0};
        update_translation(move);
    }

    if(key_released(&input.ESC)){

        player_in_start_position = false;
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
    }
    if(!player_in_start_position){
        if(player1 != NULL){
            if(player_start == NULL){
                LOG("No player start\n");
                return;
            }            

            set_element_position(player1,player_start->transform->position);
            player_in_start_position = true;
        }else{
            LOG("No player selected\n");
            return;
        }
    }
    
}

void input_gui_editor(){
    if(editor_sub_mode != EDITOR_SUB_MODE_NULL)
        return;
    can_open_text_menu_with_key(&menu_show_gui_elements,&input.L,NULL);

    if(key_released(&input.ESC)){
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        //clean_array(actual_buttons_array);
    }
        
        
    if(key__released(&input.A,GLFW_MOD_SHIFT)){
        new_empty_button();
    }

    if(key_released(&input.G)){
        change_to_editor_sub_mode(EDITOR_SUB_MODE_GRAB);
    }
}

void update_input(){
    if(editor_sub_mode == EDITOR_SUB_MODE_NULL){
        switch (editor_mode)
        {    
        case EDITOR_DEFAULT_MODE:
            default_mode();
            break;
        case EDITOR_NAVIGATE_MODE:
            navigate_mode();
            break;
        case EDITOR_GRAB_MODE:
            grab_mode();
            break;
        case EDITOR_ROTATE_MODE:
            //rotate_input_mode();
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
    
    switch (editor_sub_mode)
    {
    case EDITOR_SUB_MODE_TEXT_INPUT:
        text_input_mode();
        break;
    case EDITOR_SUB_MODE_GRAB:
        grab_mode();            
        break;
    case EDITOR_SUB_MODE_SCALE:
        
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

float last_mouse_x = 400;
float last_mouse_y = 300;
bool first_mouse_movement = true;




