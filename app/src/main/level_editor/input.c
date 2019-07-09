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


EditorMode mode_to_change;

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

unsigned char command_text_buffer[100];
bool activate_text_input_mode = false;
unsigned short int character_count = 0;


void parse_command(const char* command){
    if(command[1] == 'w'){
        save_data(&command[3]);
        printf("Level saved: %s\n",&command[3]);
    }
    if(command[1] == 'o'){        
        load_level_in_editor(&command[3]);
        printf("Level loaded\n");
    }
    if(command[1] == 'r'){        
        reload_editor();
        printf("reload\n");
    }
    if(command[1] == 'h'){        
        selected_element->has_HLOD = true;
        printf("HLOD added\n");
    }
    if(command[1] == 'd'){ 
        unsigned int duplicate_count = atoi(&command[3]); 
        for(int i = 0; i< duplicate_count; i++){
            duplicate_selected_element();        
            vec3 move = {24,0,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }      
        printf("duplicated %i\n",duplicate_count);
    }
    if(command[1] == 'h'){
        
    }
}

void parse_characters(unsigned char character){
    if(character == ':'){
        printf("Command mode\n");
        command_text_buffer[character_count] = character;
        character_count++;
        change_to_editor_mode(EDITOR_TEXT_INPUT_MODE);
    }
    if(editor_mode == EDITOR_TEXT_INPUT_MODE){
        command_text_buffer[character_count] = character;
        character_count++;
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint){
    if(codepoint == 241)//equal "ñ"
            return;
    unsigned char character[1];
    code_to_utf8(character,codepoint);
    //printf("Converted: %s\n",character);
    parse_characters(character[0]);
}

void text_input_mode(){
    if(key_released(&input.ENTER)){
        parse_command(command_text_buffer);
        character_count = 0;
        memset(command_text_buffer,0,sizeof(command_text_buffer));
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
    }
    if(key_released(&input.BACKSPACE)){
        character_count--;
        command_text_buffer[character_count] = '\0';
    }
    set_text_size(12);
    render_text(command_text_buffer , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+24) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   

}

void camera_mouse_control(float yaw, float pitch){
    vec3 front;
    
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    

    glm_normalize(front);

    glm_vec3_copy(front, camera_front);

    update_look_at();
}

void mouse_movement_control(float xpos, float ypos){
    
  
    
    horizontalAngle += camera_width_screen/2 - xpos ;
    
    verticalAngle  += camera_heigth_screen/2 - ypos ;

    horizontalAngle *= 0.05;
    verticalAngle *= 0.05;

    camera_mouse_control(0, horizontalAngle);
    

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(mouse_navigate_control)
        mouse_movement_control(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    Key* actual_key = NULL;
    Key* mod_key = NULL;
    switch (mods)
    {
    case GLFW_MOD_SHIFT:
        mod_key = &input.SHIFT;
    case GLFW_MOD_ALT:
        mod_key = &input.ALT;    
    default:
        break;
    }
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
    case GLFW_KEY_BACKSPACE:
        actual_key = &input.BACKSPACE;
    default:
        break;
    }

    if(actual_key != NULL){
        if(action == GLFW_PRESS){
            actual_key->pressed = true;
            actual_key->Released = false;
            if(mod_key != NULL)
                mod_key->pressed = true;
        }
        if(action == GLFW_RELEASE){
            actual_key->pressed = false;
            actual_key->Released = true;
            
        }
    } 
  
    
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

		if (button == GLFW_MOUSE_BUTTON_RIGHT ){
			if(action == GLFW_PRESS){
				
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
                move_camera = false;
                mouse_navigate_control = false;
                change_to_editor_mode(EDITOR_DEFAULT_MODE);
				
			}
			if(action == GLFW_RELEASE){
			
			}

		}
		if (button == GLFW_MOUSE_BUTTON_LEFT ){
			if(action == GLFW_PRESS){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                move_camera = true;
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
        change_to_editor_mode(EDITOR_GRAB_MODE);
    }

    if(key_released(&input.R)){
        change_to_editor_mode(EDITOR_ROTATE_MODE);
    }
    if(key_released(&input.V)){
        change_to_editor_mode(EDITOR_NAVIGATE_MODE);
    }
}
void change_to_editor_mode(EditorMode mode){
    if(editor_mode == mode){
        editor_mode = EDITOR_CHANGING_MODE_MODE;
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        return;
    }
    editor_mode = EDITOR_CHANGING_MODE_MODE;
    mode_to_change = mode;
    switch (mode)
    {    
    case EDITOR_DEFAULT_MODE:
        editor_mode_show_text = "Default Mode";
        break;
    case EDITOR_NAVIGATE_MODE:
        editor_mode_show_text = "Navigate Mode";
        move_camera = true;
        break;
    case EDITOR_GRAB_MODE:
        editor_mode_show_text = "Grab Mode";
        break;
    case EDITOR_ROTATE_MODE:
        editor_mode_show_text = "Rotate Mode";
        break;
    case EDITOR_CHANGING_MODE_MODE:
        editor_mode = mode_to_change;
        break;
    case EDITOR_TEXT_INPUT_MODE:
        editor_mode_show_text = "Text Input";
    default:
        break;
    } 
    memset(&input,0,sizeof(Input));
}


float move_object_value = 0.02;
bool grid_translate = false;

void grab_mode(){
    input_change_mode();

    draw_rotate_gizmo = false;
    draw_translate_gizmo = true;   
    

    if(!grid_translate){
        if(key_released(&input.KEY_1)){
            grid_translate = true;
        } 
        
        if(input.W.pressed){
            vec3 move = {0,-move_object_value,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(input.S.pressed){
            vec3 move = {0,move_object_value,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(input.D.pressed){
            vec3 move = {-move_object_value,0,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(input.A.pressed){
            vec3 move = {move_object_value,0,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);

        }
        if(input.E.pressed){
            vec3 move = {0,0,move_object_value};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(input.Q.pressed){
            vec3 move = {0,0,-move_object_value};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(key_released(&input.I)){
            move_object_value += 0.04;
        }
        if(key_released(&input.O)){
            move_object_value -= 0.04;
        }
    }else{
        if(key_released(&input.KEY_1)){
            grid_translate = false;
        } 
        
        if(key_released(&input.D)){
            vec3 move = {move_object_value,0,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(key_released(&input.A)){
            vec3 move = {-move_object_value,0,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(key_released(&input.S)){
            vec3 move = {0,-move_object_value,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(key_released(&input.W)){
            vec3 move = {0,move_object_value,0};
            glm_translate(selected_element->model->model_mat, move);
            glm_vec3_add(selected_element->position,move,selected_element->position);
        }
        if(key_released(&input.I)){
            move_object_value += 2.4;
        }
        if(key_released(&input.O)){
            move_object_value = 24;
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
        glm_vec3_mul(velocity_vector,camera_up,move);
        glm_vec3_add(camera_position,move,camera_position);
        update_look_at();
    }
    if(input.Q.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,camera_up,move);
        glm_vec3_sub(camera_position,move,camera_position);
        update_look_at();
    }
    if(input.J.pressed){
        horizontalAngle += 800/2 - rotate_value ;
        rotate_value -= 30;
        if(rotate_value < -10000){
            rotate_value = -100;
        }

        verticalAngle  += 600/2 - 100 ;

        horizontalAngle *= 0.05;
        verticalAngle *= 0.05;

        camera_mouse_control(0, horizontalAngle);
    }
    if(input.K.pressed){
         horizontalAngle += 800/2 - rotate_value ;
        rotate_value += 30;
        if(rotate_value > 10000){
            rotate_value = 100;
        }

        verticalAngle  += 600/2 - 100 ;

        horizontalAngle *= 0.05;
        verticalAngle *= 0.05;

        camera_mouse_control(0, horizontalAngle);
    }

    if(input.W.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,camera_front,move);
        glm_vec3_add(camera_position,move,camera_position);
        update_look_at();
    }
    if(input.S.pressed){
        vec3 move;
        glm_vec3_mul(velocity_vector,camera_front,move);
        glm_vec3_sub(camera_position,move,camera_position);
        update_look_at();
    }
    if(input.D.pressed){
        vec3 cross;
        glm_vec3_cross(camera_front, camera_up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_add(camera_position, move,camera_position);
        update_look_at();
    }
    if(input.A.pressed){
        vec3 cross;
        glm_vec3_cross(camera_front, camera_up, cross);
        glm_normalize(cross);
        vec3 move;
        glm_vec3_mul(velocity_vector, cross, move );
        glm_vec3_sub(camera_position, move,camera_position);
        update_look_at();
    }
    
}
void input_text_menu(TextMenu* menu, Key* open_key){
    
    if(key_released(open_key)){
        menu->execute = true;
        menu->show = true;
    }

    if(menu->show){
        if(key_released(&input.ESC)){
            menu->execute = false;
            menu->show = false;
        }
        if(key_released(&input.J)){
            menu->actual_element_select++;
        }
        if(key_released(&input.K)){
            menu->actual_element_select--;
        }
        if(key_released(&input.ENTER)){
            menu->element_selected = true;
            menu->show = false;
        }

    }
}

void default_mode(){
    draw_rotate_gizmo = false;
    draw_translate_gizmo = false;

    input_change_mode();

    if(input.SHIFT.pressed && key_released(&input.D)){
       duplicate_selected_element();
       printf("duplicated \n"); 
    }
    if(input.ALT.pressed && key_released(&input.A)){
        printf("deselect all \n");
        deselect_all(); 
    }
    if(key_released(&input.S)){
        get_element_status(selected_element);
    }
    
  
    if(key_released(&input.X)){
        remove_selected_element();            
    } 
    
    
    if(key_released(&input.Q)){
        get_elements_in_editor_map();
    }

    if(key_released(&input.KEY_1)){
        if(can_draw_gizmos){
            can_draw_gizmos = false;
        }else can_draw_gizmos = true;

    }
    if(key_released(&input.KEY_2)){
        init_skeletal_gizmo();
        can_draw_skeletal_bones = true;
    }
    
        
    input_text_menu(&add_element_menu,&input.A);
    input_text_menu(&editor_element_list_menu,&input.L);
    input_text_menu(&add_texture_menu,&input.T);  

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
    if(key_released(&input.J)){
       rotate_editor_element(selected_element, -5, (vec3){0,0,1});
    }
    if(key_released(&input.K)){
        
        rotate_editor_element(selected_element, 5, (vec3){0,0,1});
    }
}



void update_input(){
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
        rotate_input_mode();
        break;
    case EDITOR_TEXT_INPUT_MODE:
        text_input_mode();
        break;
    case EDITOR_CHANGING_MODE_MODE:
        editor_mode = mode_to_change;
        break;
    default:
        break;
    } 
   
}

float last_mouse_x = 400;
float last_mouse_y = 300;
bool first_mouse_movement = true;




