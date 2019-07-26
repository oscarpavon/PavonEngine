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
Array editor_textures;

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
    current_component_selected = NULL;
}

void editor_message(const char* message){
    set_text_size(12);
    render_text(message , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+12) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   
}

void add_editor_native_element(const char* native_element_name){
    
    if( strcmp("Camera", native_element_name) == 0 ){        
        new_empty_element();        
        strcpy(selected_element->name, "Camera01");              
        
        add_transform_component_to_selected_element();        

        CameraComponent camera_component;
        init_camera_component(&camera_component);
        add_component_to_selected_element(sizeof(CameraComponent), &camera_component, CAMERA_COMPONENT);
        
    }else if ( strcmp("Player Start", native_element_name) == 0 )
    {        
        new_empty_element();
        strcpy(selected_element->name, "PlayerStart01");        
              
        player_start = selected_element;
        add_transform_component_to_selected_element();        
        selected_model = NULL;
        
    }else if ( strcmp("Player Controller", native_element_name) == 0 )
    {
        new_empty_element();
        strcpy(selected_element->name, "Player1");
        
    }else if ( strcmp("Sphere", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "Sphere");
        add_transform_component_to_selected_element();
        SphereComponent sphere;
        memset(&sphere,0,sizeof(SphereComponent));
        init_sphere_component(&sphere);
        add_component_to_selected_element(sizeof(SphereComponent), &sphere, SPHERE_COMPONENT);
    }
    else if ( strcmp("Cube", native_element_name) == 0 ){
        new_empty_element();
        strcpy(selected_element->name, "Cube");
        add_transform_component_to_selected_element();
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
        add_transform_component_to_selected_element();
        StaticMeshComponent mesh_component;
        //mesh_component.model = get_from_array(&engine_native_models,3);
        add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);

    }
}


void clean_editor(){   
    
    clean_skeletal_editor();

    clear_engine_memory();
}


void rotate_editor_element(Element* element, float angle, vec3 axis){
   TransformComponent* transform = get_component_from_element(element, TRASNFORM_COMPONENT);
   if(transform == NULL){
       LOG("No transfrom pointer in element\n");
       return;
   }
    versor new_rot_quat;
    glm_quatv(new_rot_quat, glm_rad(angle), axis);

    versor result_quat;
    glm_quat_mul(transform->rotation, new_rot_quat, result_quat);

    glm_quat_copy(result_quat, transform->rotation);

    mat4 model_rot_mat;
    glm_quat_mat4(new_rot_quat,model_rot_mat);

    glm_mul(transform->model_matrix,model_rot_mat, transform->model_matrix);
    
}

void rotate_editor_selected_element_with_quaternion(versor quaternion){
    mat4 model_rot_mat;
    glm_quat_mat4(quaternion,model_rot_mat);

    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(transform)
        glm_mul(transform->model_matrix,model_rot_mat, transform->model_matrix);
}

void update_camera_aspect_ratio(){
    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , main_camera.projection);
}


void load_level_in_editor(const char* name){
    strcpy(opened_file_name,name);

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

    int level_result = load_level_to_elements_array(save_name, actual_elements_array);
    if(level_result != 0)
        return;   
    
    actual_model_array = &array_models_loaded;
    for(int i = 0; i< texts.count ; i++){
       load_and_initialize_simple_model(get_from_array(&texts,i));
    }    
    actual_model_array = &editor_models;

    for(int i = 0; i< textures_paths.count ; i++){
       load_simple_image(get_from_array(&textures_paths,i));
    } 

    for_each_element_components(&init_element_component);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec result = diff(time1,time2);
    long millisecond = result.tv_nsec / 1000000;
    LOG("Level loading time: %ld ms\n",millisecond);
} 

void duplicate_selected_element(){
    Element* original_elmenent = selected_element;    
    new_empty_element();
    for(int i = 0; i < original_elmenent->components.count ; i++){
        ComponentDefinition* component_definition = get_from_array(&original_elmenent->components,i);
        switch (component_definition->type)
        {
        case TRASNFORM_COMPONENT:
            add_transform_component_to_selected_element();
            TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
            memcpy(transform,original_elmenent->transform, sizeof(TransformComponent));
            break;
        case CAMERA_COMPONENT:
            add_camera_component_to_selected_element();
            break;
        case STATIC_MESH_COMPONENT:{
            StaticMeshComponent* original_mesh = get_component_from_element(original_elmenent,STATIC_MESH_COMPONENT);
            StaticMeshComponent new_mesh;
            memset(&new_mesh,0,sizeof(StaticMeshComponent));
            memcpy(&new_mesh,original_mesh,sizeof(StaticMeshComponent));
            memset(&new_mesh.meshes,0,sizeof(Array));
            init_array(&new_mesh.meshes,sizeof(unsigned int),original_mesh->meshes.count);
            unsigned int* model_id_in_loaded_model = get_from_array(&original_mesh->meshes,0);
            add_to_array(&new_mesh.meshes, model_id_in_loaded_model);
            for(int i = 1 ; i < original_mesh->meshes.count ; i++){
                new_empty_model();
                unsigned int* original_model_id = get_from_array(&original_mesh->meshes,i);
                Model* original_model = get_from_array(actual_model_array,*original_model_id);
                duplicate_model_data(selected_model,original_model);
                selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);                
                unsigned int new_id = actual_model_array->count -1;
                add_to_array(&new_mesh.meshes, &new_id);
            }  
            add_component_to_selected_element(sizeof(StaticMeshComponent),&new_mesh,STATIC_MESH_COMPONENT);
        }          
        break;

        default:
            break;
        }
    }    
    

}

void remove_selected_element(){
    selected_element = NULL;
    element_id_count--;
}

void reload_editor(){
    element_id_count = 0;
    
    clean_array(&editor_elements);   
    clean_array(&editor_models);
    clean_array(&texts);
    clean_array(&textures_paths);
    clean_array(actual_buttons_array);
    clean_array(&array_models_loaded);
}

void init_editor(){
    
    actual_model_array = &editor_models;
    actual_elements_array = &editor_elements;
    current_textures_array = &editor_textures;
    

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
    init_array(&editor_textures, sizeof(Texture),100);
    
    

    element_id_count = 0;    

    editor_mode = EDITOR_DEFAULT_MODE;
    editor_sub_mode = EDITOR_SUB_MODE_NULL;
    
    editor_mode_show_text = "Default Mode";
    editor_sub_mode_text = "";

    init_input();

    camera_velocity = 0.04;    

    //init_skeletal_editor();
    
    

}

void draw_count_of_draw_call(){
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text("Draw:" , 0 + ((camera_width_screen/2)-500) * pixel_size_x , 0 + ((camera_heigth_screen/2)-20) * pixel_size_y  , pixel_size_x, pixel_size_y, false);  
    char buf[5]; 
    float count = frame_draw_elements.count;
    gcvt(count, 6, buf);
    if(count != 0)
    render_text(buf , 0 + ((camera_width_screen/2)-440) * pixel_size_x , 0 + ((camera_heigth_screen/2)-20) * pixel_size_y  , pixel_size_x, pixel_size_y, false);  

}

void draw_tringles_count(){
    int triangles = 0;
    for(int i = 0; i<frame_draw_elements.count; i++){
        Model** model = get_from_array(&frame_draw_elements,i);
        int vertices_count = model[0]->vertex_array.count;
        triangles += vertices_count/3;
    }

    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text("Tris:" , 0 + ((camera_width_screen/2)-600) * pixel_size_x , 0 + ((camera_heigth_screen/2)-20) * pixel_size_y  , pixel_size_x, pixel_size_y, false);  
    char buf[7]; 
    float count = triangles;
    gcvt(count, 7, buf);
    if(count != 0)
    render_text(buf , 0 + ((camera_width_screen/2)-560) * pixel_size_x , 0 + ((camera_heigth_screen/2)-20) * pixel_size_y  , pixel_size_x, pixel_size_y, false);  

}

void draw_editor_viewport(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(controlling_camera_component){
        CameraComponent* camera = get_component_from_selected_element(CAMERA_COMPONENT);
        update_main_camera_with_camera_component_values(camera);
    }

    for_each_element_components(&update_per_frame_component);
    test_elements_occlusion();
    check_static_meshes_distance();

    draw_count_of_draw_call();
    
    draw_tringles_count();


    draw_elements(&frame_draw_elements);
    clean_array(&models_for_test_occlusion);
    clean_array(&array_static_meshes_pointers);
    clean_array(&array_static_meshes_pointers_for_test_distance);
    for_each_element_components(&clean_component_value);

    draw_gizmos();

    if(editor_mode == EDITOR_MODE_GUI_EDITOR || editor_mode == EDITOR_PLAY_MODE  ){
         draw_gui();
         
    }       

    text_renderer_loop();

    editor_message("editor message");       

}
