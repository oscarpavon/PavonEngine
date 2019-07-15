#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <string.h>
#include "../vector.h"

#include "../engine.h"

#include "text.h"

#include "../camera.h"

#include "../images.h"

#include "skeletal_editor.h"

#include "../Engine/level.h"

Array editor_models;

Array LOD_models;

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

void play_game_standalone(){
    int exit_status = system("st sh ../level_editor/compile_game.sh");
}

void deselect_all(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = get_element_from_array(&editor_elements,i);
        element->selected = false;
    }
}

void editor_message(const char* message){
    set_text_size(12);
    render_text(message , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+12) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   
}

void add_editor_native_element(const char* native_element_name){
    
    if( strcmp("Camera", native_element_name) == 0 ){
        new_empty_model();
        new_empty_element();
        selected_model->draw = false;
        strcpy(selected_element->name, "Camera01");
        selected_element->type = ELEMENT_TYPE_CAMERA;
        ElementComponent camera_component;
        memset(&camera_component,0,sizeof(ElementComponent));
        add_element_to_array(&components,&camera_component);
        ElementComponent * component = get_element_from_array(&components,0);
        selected_element->components = component;
        component->id = 0;
        component->bytes_size = sizeof(CameraComponent);
        component->data = malloc(component->bytes_size);
        CameraComponent* camera = component->data;
        glm_vec3_copy((vec3){0,1,0},camera->front);
        glm_mat4_copy(main_camera.projection,camera->projection);
        vec3 look_pos;
        glm_vec3_add(selected_element->position, camera->front, look_pos);
        glm_lookat(selected_element->position, look_pos, camera_up , camera->view);
        
    }else if ( strcmp("Player Start", native_element_name) == 0 )
    {
        new_empty_model();
        new_empty_element();
        selected_model->draw = false;
        strcpy(selected_element->name, "PlayerStart01");
        selected_element->type = ELEMENT_TYPE_PLAYER_START;
        player_start = selected_element;
        selected_element = NULL;
        selected_model = NULL;
        
    }else if ( strcmp("Player Controller", native_element_name) == 0 )
    {
        new_empty_element();
        selected_element->model = selected_model;
        strcpy(selected_element->name, "Player1");
        selected_element->type = ELEMENT_TYPE_PLAYER_CONTROLLER;
    }
}


void clean_element(Element* element){    
    if(element->model->skeletal != NULL){
        free(element->model->skeletal->joints);
        free(element->model->skeletal);        
    }
}

void clean_editor(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        clean_element(element);
        
    }
    
    clean_skeletal_editor();

    clear_engine_memory();
}


void rotate_editor_element(Element* element, float angle, vec3 axis){
   
    versor new_rot_quat;
    glm_quatv(new_rot_quat, glm_rad(angle), axis);

    versor result_quat;
    glm_quat_mul(element->rotation, new_rot_quat, result_quat);

    glm_quat_copy(result_quat, element->rotation);

    mat4 model_rot_mat;
    glm_quat_mat4(new_rot_quat,model_rot_mat);

    glm_mul(element->model->model_mat, model_rot_mat, element->model->model_mat);

}

void update_camera_aspect_ratio(){
    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , main_camera.projection);
}

Array load_elements;

void load_level_in_editor(const char* name){
    struct timespec time1, time2;
    int temp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    char* level_folder = "../Game/levels/";
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, level_folder);
    strcat(save_name,name);
    strcat(save_name,".lvl");
    printf("%s\n",save_name);

    load_level_to_elements_array(save_name, &load_elements);   
    
    add_loaded_elements(&load_elements, &editor_models, &editor_elements);
    clean_array(&load_elements);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec result = diff(time1,time2);
    long millisecond = result.tv_nsec / 1000000;
    printf("Level loading time: %ld ms\n",millisecond);
}

void duplicate_selected_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));
    

    Model new_model;
    memset(&new_element,0,sizeof(struct Model));

    memcpy(&new_model,selected_element->model,sizeof(struct Model));
    new_model.has_HLOD = false;
    new_model.HLOD = NULL;
    new_model.change_to_HLOD = false;
    add_element_to_array(&editor_models,&new_model);
    
    element_id_count++;
    
    memcpy(&new_element, selected_element, sizeof(struct Element));

    new_element.model = get_element_from_array(&editor_models,editor_models.count-1);
    new_element.id = element_id_count;
    new_element.has_HLOD = false;

    if(selected_element->duplicated_of_id > -1)
        new_element.duplicated_of_id = selected_element->duplicated_of_id;
    else
        new_element.duplicated_of_id = selected_element->id;

    add_element_to_array(&editor_elements,&new_element);
    
    select_last_element();
}

void get_elements_in_editor_map(){
    printf("Elements count: %i\n", editor_elements.count);

    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        printf("Element name: %s\n", element->model_path);
    }
  

}
void get_element_status(Element* element){
    printf("Position: %f, %f %f\n",element->position[0] , element->position[1] , element->position[2]);

}

void remove_selected_element(){
    remove_last_element_from_model_array(&editor_models);
    clean_element(selected_element);
    selected_element = NULL;
    remove_element_from_array(&editor_elements);
    element_id_count--;
}

void reload_editor(){
    element_id_count = 0;
    
    clean_array(&editor_elements);   
    
}

void init_editor(){
    
    actual_model_array = &editor_models;
    actual_elements_array = &editor_elements;
    actual_LOD_models_array = &LOD_models;
    

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    init_vec3(-6,0,2, camera_position);
    update_look_at();

    init_array_with_count(&editor_models, sizeof(Model),100);
    init_array_with_count(&LOD_models,sizeof(Model),100);

    init_array_with_count(&editor_elements,sizeof(Element),100);

    init_gizmos();

    editor_standard_fragment_shader = compile_shader(editor_standard_fragment_shader_source, GL_FRAGMENT_SHADER);

    init_text_renderer();    
    
    init_array_with_count(&selected_elements_id,sizeof(unsigned short int),100);

    element_id_count = 0;    

    editor_mode = EDITOR_DEFAULT_MODE;
    editor_sub_mode = EDITOR_SUB_MODE_NULL;
    
    editor_mode_show_text = "Default Mode";
    editor_sub_mode_text = "";

    init_input();

    camera_velocity = 0.04;    

    init_skeletal_editor();
    
    actual_standard_fragment_shader = editor_standard_fragment_shader;
}


void check_elements_camera_distance_for_LOD(){
    for(int i = 0; i< editor_elements.count ; i++){
        Element* element = get_element_from_array(&editor_elements,i);
        if(element->has_LOD){
            float camera_distance = glm_vec3_distance(element->position, camera_position);
            //printf("Camera Distance: %f\n",camera_distance);
            if(camera_distance >= 8){
                element->model->change_LOD  = true;
            }else if (camera_distance <= 7.6){
                element->model->change_LOD  = false;
            }
            if(camera_distance >= 20 && element->model->has_HLOD){
                element->model->change_to_HLOD = true;
            }
            if(camera_distance < 19){
                element->model->change_to_HLOD = false;
            }
        }        
    }
}

void assign_LOD_mesh(){
    for(int i = 0; i < editor_models.count ; i++){
        struct Model* model = get_element_from_array(&editor_models,i);
        if(model->change_LOD){
            model->LOD = &LOD_models.data[model->actual_LOD];
        }
    }
}

void init_game_in_editor(){
    
}

void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    check_elements_camera_distance_for_LOD();
    assign_LOD_mesh();
    
    draw_elements(&editor_elements);  
    
    glClear(GL_DEPTH_BUFFER_BIT);

    draw_gizmos();

    if(editor_mode == EDITOR_MODE_GUI_EDITOR)
        draw_gui();

    text_renderer_loop();

    editor_message("editor message");    

}
