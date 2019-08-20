#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "windows_manager.h"

#include "../engine.h"

#include "text.h"

#include "../camera.h"

#include "../images.h"

#include "skeletal_editor.h"

#include "../Engine/level.h"

#include "geometry.h"

#include "Textures/texture_factory.h"

#include "HLOD/HLOD_factory.h"


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

void editor_add_HLOD_element(HLODCluster* cluster){
    
    new_empty_element();
    char name[20];
    sprintf(name,"HLODCluster_%i",cluster->id);
    strcpy(selected_element->name, name);              
    add_transform_component_to_selected_element();
    component_add_HLOD_to_select_element();
    HLODComponent* hlod = get_component_from_selected_element(COMPONENT_HLOD);

    init_array(&hlod->childs,sizeof(void*),cluster->elements.count);
    for(int i = 0; i<cluster->elements.count; i++){
        Element** ppElement = get_from_array(&cluster->elements,i);        
        Element* element = ppElement[0];
        add_to_array(&hlod->childs, &element);
        element->proccess = false;
    }

    hlod->distance = 50;
    glm_aabb_center(hlod->bounding_box,hlod->center);
    hlod->has_childs_HLOD = false;

    char path[40];
    sprintf(path,"HLOD/HLOD_out%i.gltf",cluster->id);
    load_model(path);
    Model* original = selected_model;
    new_empty_model();
    duplicate_model_data(selected_model, original);
    hlod->model = selected_model;
    Texture new_texture;
    sprintf(path,"HLOD/HLOD_texture%i.png",cluster->id);
    texture_load(path,&new_texture);
    hlod->model->texture.id = new_texture.id;
}

void editor_generate_and_add_cube_element(){
    new_empty_element();
    add_transform_component_to_selected_element();

    StaticMeshComponent mesh_component;    
    init_array(&mesh_component.meshes,sizeof(unsigned int),2);
    init_array(&mesh_component.textures,sizeof(unsigned int),2);
    int model_path_id = texts.count-1;
    add_to_array(&mesh_component.meshes,&model_path_id);

    new_empty_model();
    add_to_array(&mesh_component.meshes,&selected_model->id);

    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    geometry_cube_create_indices();
    vec3 box[2];
    glm_vec3_copy(VEC3(-1,-1,-1),box[0]);
    glm_vec3_copy(VEC3(1,1,1),box[1]);
    
    geometry_sphere_create_vertices(64);

    init_model_gl_buffers(selected_model); 
    
    add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);


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
        add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);

    }else if( strcmp("HLOD Cluster",native_element_name) == 0){
        new_empty_element();
        strcpy(selected_element->name, "HLOD Cluster");
        add_transform_component_to_selected_element();
        HLODBoxComponent box_component;
        memset(&box_component,0,sizeof(HLODBoxComponent));
        glm_vec3_copy(VEC3(1,1,1),box_component.bounding_box[0]);
        glm_vec3_copy(VEC3(-1,-1,-1),box_component.bounding_box[1]);
        add_component_to_selected_element(sizeof(HLODBoxComponent), &box_component, COMPONENT_HLOD_BOX);
        HLOD_clusters[HLOD_cluster_count] = selected_element;
        HLOD_cluster_count++;
    }
}


void clean_editor(){     
    
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

    for(int i = 0; i<selected_element->components.count; i++){
        ComponentDefinition* component = get_from_array(&selected_element->components,i);
        update_component(component);
    }
    
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


void editor_load_level(const char* name){
    strcpy(opened_file_name,name);

    struct timespec time1, time2;
    int temp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    char new_file_name_with_path[strlen(pavon_the_game_project_folder) + 40];
    sprintf(new_file_name_with_path,"%s%s%s.lvl",pavon_the_game_project_folder,"Content/levels/",name);

    int level_result = load_level_to_elements_array(new_file_name_with_path, actual_elements_array);
    if(level_result != 0)
        return;   
    
    actual_model_array = &array_models_loaded;
    for(int i = 0; i< texts.count ; i++){
        char* model_path = get_from_array(&texts,i);
        load_model(model_path);
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

void duplicate_selected_element(int current_count, Element* original){
    Element* last_copy = selected_element;
    new_empty_element();
    char new_name[30];
    sprintf(new_name,"%s%i",original->name,current_count);
    strcpy(selected_element->name,new_name);
    for(int i = 0; i < last_copy->components.count ; i++){
        ComponentDefinition* component_definition = get_from_array(&last_copy->components,i);
        switch (component_definition->type)
        {
        case TRASNFORM_COMPONENT:
            add_transform_component_to_selected_element();
            TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
            memcpy(transform,last_copy->transform, sizeof(TransformComponent));
            break;
        case CAMERA_COMPONENT:
            add_camera_component_to_selected_element();
            break;
        case STATIC_MESH_COMPONENT:{
            StaticMeshComponent* original_mesh = get_component_from_element(last_copy,STATIC_MESH_COMPONENT);
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

void editor_add_element_with_model_path(const char* path){
    add_element_with_model_path(path);
    update_translation(main_camera.position);
    vec3 new_position;
    glm_vec3_scale(main_camera.front,4,new_position);
    update_translation(new_position);

    
    StaticMeshComponent* mesh_component = get_component_from_selected_element(STATIC_MESH_COMPONENT); 
    if(!mesh_component)
        return;     
               
    for(int i = 1; i<=mesh_component->meshes.count-1 ; i++){                

        u8* id = get_from_array(&mesh_component->meshes,i);
        Model* model = get_from_array(actual_model_array,*id);
        model->texture.id = editor_texture_checker.id;

    }
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

    texture_load("../NativeContent/Editor/checker_texture.png",&editor_texture_checker);
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

void editor_focus_selected_element(){
    if(!selected_element)
        return;
    vec3 new_position;
    glm_vec3_copy(VEC3(selected_element->transform->position[0],
                    selected_element->transform->position[1] - 4,
                    selected_element->transform->position[2]),main_camera.position);
    vec3 direction;
    glm_vec3_sub(selected_element->transform->position, main_camera.position,direction);
    glm_normalize_to(direction,main_camera.front);
    update_look_at();
}


void collision_test(){
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
        return;
    if(input.C.pressed){

        Element* element1 = get_from_array(actual_elements_array,0);
        if(!element1)
            return;
        

        StaticMeshComponent* mesh = get_component_from_element(element1,STATIC_MESH_COMPONENT);
        unsigned int* modelid = get_from_array(&mesh->meshes,1);

        Element* element2 = get_from_array(actual_elements_array,1);
        if(!element2)
            return;
        StaticMeshComponent* mesh2 = get_component_from_element(element2,STATIC_MESH_COMPONENT);
        unsigned int* modelid2 = get_from_array(&mesh2->meshes,1);

        Model* model1 = get_from_array(actual_model_array,*modelid);
        Model* model2 = get_from_array(actual_model_array,*modelid2);
    
        if( collision_of(model1,model2) ){
            LOG("collision SAT\n");
            
        }
        else{
            LOG("NO SAT\n");
        }
    }


}

void draw_editor_viewport(){

    if( window_editor_main.focus){
       

        if(is_editing_blender_file){
            if(!blender_file_checked){
            int result = system("blender -b ../assets/test_blender.blend --python-exit-code 2 --disable-abort-handler -P ../scripts/Blender/check_is_saved.py");
            if(result != 0){
                LOG("Edited \n");
            }
            if(is_blender_file_saved){
                LOG("reimport mesh blender file\n");
            }
            blender_file_checked = true;
            }
            
        }
    }else{
        blender_file_checked = false;
       
    }

    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(isDrawUV)
        draw_UV();
    
    

    if(controlling_camera_component){
        CameraComponent* camera = get_component_from_selected_element(CAMERA_COMPONENT);
        update_main_camera_with_camera_component_values(camera);
    }

    for_each_element_components(&update_per_frame_component);
    Model* draw_model = get_from_array(actual_model_array,1);
    if(draw_model){
       
        //draw_vertices_like(GL_POINTS,draw_model,(vec4){1,0,0,0});
       // draw_simgle_model(draw_model);
    }
    

    test_elements_occlusion();
    check_meshes_distance();

    draw_count_of_draw_call();
    
    draw_tringles_count();

    play_animation_list();
    if(update_vertex_bones_gizmos)
        update_joints_vertex();

    draw_elements(&frame_draw_elements);

    //clean frame
    clean_array(&models_for_test_occlusion);
    clean_array(&array_static_meshes_pointers);
    clean_array(&array_static_meshes_pointers_for_test_distance);
    clean_array(&array_skinned_mesh_pointers);
    clean_array(&array_skinned_mesh_for_distance_test);
    for_each_element_components(&clean_component_value);
    //end clean frame

    draw_gizmos();

    if(editor_mode == EDITOR_MODE_GUI_EDITOR || editor_mode == EDITOR_PLAY_MODE  ){
         draw_gui();         
    }       

    text_renderer_loop();

    //editor_message("editor message");       

    //collision_test();

    
}
