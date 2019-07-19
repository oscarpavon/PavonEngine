#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <string.h>
#include "../Engine/array.h"

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
        Element* element = get_from_array(&editor_elements,i);
        element->selected = false;
    }
}

void editor_message(const char* message){
    set_text_size(12);
    render_text(message , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+12) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   
}

void add_editor_native_element(const char* native_element_name){
    
    if( strcmp("Camera", native_element_name) == 0 ){        
        new_empty_element();        
        strcpy(selected_element->name, "Camera01");
        selected_element->type = ELEMENT_TYPE_CAMERA;        
        
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);

        CameraComponent camera_component;
        init_camera_component(&camera_component);
        add_component_to_selected_element(sizeof(CameraComponent), &camera_component, CAMERA_COMPONENT);
        
    }else if ( strcmp("Player Start", native_element_name) == 0 )
    {        
        new_empty_element();
        strcpy(selected_element->name, "PlayerStart01");        
        selected_element->type = ELEMENT_TYPE_PLAYER_START;        
        player_start = selected_element;
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);        
        
        selected_model = NULL;
        
    }else if ( strcmp("Player Controller", native_element_name) == 0 )
    {
        new_empty_element();
        selected_element->model = selected_model;
        strcpy(selected_element->name, "Player1");
        selected_element->type = ELEMENT_TYPE_PLAYER_CONTROLLER;
    }else if ( strcmp("Sphere", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "Sphere");
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);
        SphereComponent sphere;
        memset(&sphere,0,sizeof(SphereComponent));
        init_sphere_component(&sphere);
        add_component_to_selected_element(sizeof(SphereComponent), &sphere, SPHERE_COMPONENT);
    }
    else if ( strcmp("Cube", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "Cube");
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);
        CubeComponent cube;
        memset(&cube,0,sizeof(SphereComponent));
        init_cube_component(&cube);
        add_component_to_selected_element(sizeof(CubeComponent), &cube, CUBE_COMPONENT);
    }else if ( strcmp("Empty Element", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "New Empty Element");
    }else if ( strcmp("Floor", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "Floor");
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);

        StaticMeshComponent mesh_component;
        mesh_component.model = get_from_array(&engine_native_models,3);
        add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);

    }
}


void clean_element(Element* element){    
    if(element->model->skeletal != NULL){
             
    }
}

void clean_editor(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_from_array(&editor_elements,i);
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

    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(transform)
        glm_mul(transform->model_matrix,model_rot_mat, transform->model_matrix);
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
    LOG("%s\n",save_name);

    int level_result = load_level_to_elements_array(save_name, &load_elements);
    if(level_result != 0)
        return;   
    
    add_loaded_elements(&load_elements, &editor_models, &editor_elements);
    //clean_array(&load_elements);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec result = diff(time1,time2);
    long millisecond = result.tv_nsec / 1000000;
    LOG("Level loading time: %ld ms\n",millisecond);
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
    add_to_array(&editor_models,&new_model);
    
    element_id_count++;
    
    memcpy(&new_element, selected_element, sizeof(struct Element));

    new_element.model = get_from_array(&editor_models,editor_models.count-1);
    new_element.id = element_id_count;
    new_element.has_HLOD = false;

    if(selected_element->duplicated_of_id > -1)
        new_element.duplicated_of_id = selected_element->duplicated_of_id;
    else
        new_element.duplicated_of_id = selected_element->id;

    add_to_array(&editor_elements,&new_element);
    
    select_last_element();
}

void get_elements_in_editor_map(){
    LOG("Elements count: %i\n", editor_elements.count);

    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_from_array(&editor_elements,i);
        LOG("Element name: %s\n", element->model_path);
    }
  

}
void get_element_status(Element* element){
    LOG("Position: %f, %f %f\n",element->position[0] , element->position[1] , element->position[2]);

}

void remove_selected_element(){
    //remove_last_element_from_model_array(&editor_models);
    clean_element(selected_element);
    selected_element = NULL;
   // remove_element_from_array(&editor_elements);
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
    
    init_vec3(-6,0,2, main_camera.position);
    update_look_at();   

    init_gizmos();

    editor_standard_fragment_shader = compile_shader(editor_standard_fragment_shader_source, GL_FRAGMENT_SHADER);

    init_text_renderer();    
    
    init_array(&selected_elements_id,sizeof(unsigned short int),100);
    init_array(&LOD_models,sizeof(Model),10);
    init_array(&editor_elements,sizeof(Element),100);
    init_array(&editor_models, sizeof(Model),100);
    init_array(&engine_native_models,sizeof(Model),10);
    

    element_id_count = 0;    

    editor_mode = EDITOR_DEFAULT_MODE;
    editor_sub_mode = EDITOR_SUB_MODE_NULL;
    
    editor_mode_show_text = "Default Mode";
    editor_sub_mode_text = "";

    init_input();

    camera_velocity = 0.04;    

    //init_skeletal_editor();
    
    
    actual_standard_fragment_shader = editor_standard_fragment_shader;    
    load_model_to_array(&engine_native_models,"editor/sphere.glb", "editor/sphere_diffuse.png");
    load_model_to_array(&engine_native_models,"editor/cube.glb", "editor/cube_diffuse.jpg");
    load_model_to_array(&engine_native_models,"editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_model_to_array(&engine_native_models,"editor/floor.glb", "editor/floor.jpg");

}


void check_elements_camera_distance_for_LOD(){
    for(int i = 0; i< editor_elements.count ; i++){
        Element* element = get_from_array(&editor_elements,i);
        if(element->has_LOD){
            float camera_distance = glm_vec3_distance(element->position, main_camera.position);
            //LOG("Camera Distance: %f\n",camera_distance);
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
        struct Model* model = get_from_array(&editor_models,i);
        if(model->change_LOD){
            model->LOD = &LOD_models.data[model->actual_LOD];
        }
    }
}

void init_game_in_editor(){
    
}

void update_elements_components(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = get_from_array(&editor_elements,i);
        if(element->components_count > 0){
            for(int o = 0; o < element->components_count ; o++){
                ComponentDefinition* component = get_from_array(&element->components,o);
                update_component(component);
            }
        }
    }
}

void draw_editor_viewport(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //check_elements_camera_distance_for_LOD();
    //assign_LOD_mesh();
    
    update_elements_components();

    draw_elements(&frame_draw_elements);

    draw_gizmos();

    if(editor_mode == EDITOR_MODE_GUI_EDITOR || editor_mode == EDITOR_PLAY_MODE  ){
         update_user_iterface_status();
         draw_gui();
         
    }       

    text_renderer_loop();

    editor_message("editor message");       

}
