#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "../Engine/engine.h"

#include "text.h"

#include "../Engine/camera.h"

#include "../Engine/images.h"

#include "skeletal_editor.h"

#include "../Engine/level.h"

#include "geometry.h"

#include "Textures/texture_factory.h"

#include "HLOD/HLOD_factory.h"

#include "EditServer/edit_server.h"

#include "file_explorer.h"
#include "Windows/content_browser.h"
Array editor_models;
Array editor_textures;

Array LOD_models;


void play_game_standalone(){
    int exit_status = system("st sh ../level_editor/compile_game.sh");
}

void deselect_all(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = array_get(&editor_elements,i);
        element->selected = false;
    }
    current_component_selected = NULL;
}

void editor_message(const char* message){
    set_text_size(12);
    text_render(message , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+12) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   
}

void editor_add_HLOD_element(HLODCluster* cluster){
    
    new_empty_element();
    char name[20];
    sprintf(name,"HLODCluster_%i",cluster->id);
    strcpy(selected_element->name, name);              
    add_transform_component_to_selected_element();
    component_add_HLOD_to_select_element();
    HLODComponent* hlod = get_component_from_selected_element(COMPONENT_HLOD);

    array_init(&hlod->childs,sizeof(void*),cluster->elements.count);
    for(int i = 0; i<cluster->elements.count; i++){
        Element** ppElement = array_get(&cluster->elements,i);        
        Element* element = ppElement[0];
        array_add(&hlod->childs, &element);
        element->proccess = false;
    }

    hlod->distance = 50;
    glm_aabb_center(hlod->bounding_box,hlod->center);
    hlod->has_childs_HLOD = false;

    char path[strlen(pavon_the_game_project_folder) + 40];
    sprintf(path,"%sContent/HLOD/HLOD_out%i.gltf",pavon_the_game_project_folder,cluster->id);
    load_model(path);
    Model* original = selected_model;
    new_empty_model();
    duplicate_model_data(selected_model, original);
    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
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
    array_init(&mesh_component.meshes,sizeof(unsigned int),2);
    array_init(&mesh_component.textures,sizeof(unsigned int),2);
    int model_path_id = texts.count-1;
    array_add(&mesh_component.meshes,&model_path_id);

    new_empty_model();
    array_add(&mesh_component.meshes,&selected_model->id);

    selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
    geometry_cube_create_indices();
    vec3 box[2];
    glm_vec3_copy(VEC3(-1,-1,-1),box[0]);
    glm_vec3_copy(VEC3(1,1,1),box[1]);
    
    geometry_sphere_create_vertices(64);

    GPU_buffers_create_for_model(selected_model); 
    
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


void editor_finish(){
    editor_running = false;
    engine_running = false;   
    clear_engine_memory();
    edit_server_finish();
	
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
        ComponentDefinition* component = array_get(&selected_element->components,i);
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
        char* model_path = array_get(&texts,i);
        load_model(model_path);
    }    
    actual_model_array = &editor_models;

    for(int i = 0; i< textures_paths.count ; i++){
       load_simple_image(array_get(&textures_paths,i));
    } 

    for_each_element_components(&init_element_component);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec result = time_diffence(time1,time2);
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
        ComponentDefinition* component_definition = array_get(&last_copy->components,i);
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
            array_init(&new_mesh.meshes,sizeof(u8),original_mesh->meshes.count);
            u8* model_id_in_loaded_model = array_get(&original_mesh->meshes,0);
            array_add(&new_mesh.meshes, model_id_in_loaded_model);
            for(int i = 1 ; i < original_mesh->meshes.count ; i++){
                new_empty_model();
                u8* original_model_id = array_get(&original_mesh->meshes,i);
                Model* original_model = array_get(actual_model_array,*original_model_id);
                duplicate_model_data(selected_model,original_model);
                selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);                
                u8 new_id = actual_model_array->count -1;
                array_add(&new_mesh.meshes, &new_id);
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
    
    array_clean(&editor_elements);   
    array_clean(&editor_models);
    array_clean(&texts);
    array_clean(&textures_paths);
    array_clean(actual_buttons_array);
    array_clean(&array_models_loaded);
}

void editor_update_command_queue(){

}

void editor_init_new_added_element(){
    
    strcpy(selected_element->name, "New Element");

    update_translation(main_camera.position);
    vec3 new_position;
    glm_vec3_scale(main_camera.front,4,new_position);
    update_translation(new_position);

    
    StaticMeshComponent* mesh_component = get_component_from_selected_element(STATIC_MESH_COMPONENT); 
    if(!mesh_component)
        return;     
               
    for(int i = 1; i<=mesh_component->meshes.count-1 ; i++){                

        u8* id = array_get(&mesh_component->meshes,i);
        Model* model = array_get(actual_model_array,*id);
        model->texture.id = editor_texture_checker.id;

    }
}

void editor_add_element_with_model_path(const char* path){

    add_element_with_model_path(path);

    editor_init_new_added_element();
}


void editor_stats_calculates_triangles(){
    int triangles = 0;
    for(int i = 0; i<frame_draw_elements.count; i++){
        Model** model = array_get(&frame_draw_elements,i);
        if(!model)
            return;
        int vertices_count = model[0]->vertex_array.count;
        triangles += vertices_count/3;
    }
	editor_stats_triangles = triangles;

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
    camera_update(&current_window->camera);
}


void collision_test(){
    if(editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
        return;
    if(input.C.pressed){

        Element* element1 = array_get(actual_elements_array,0);
        if(!element1)
            return;
        

        StaticMeshComponent* mesh = get_component_from_element(element1,STATIC_MESH_COMPONENT);
        unsigned int* modelid = array_get(&mesh->meshes,1);

        Element* element2 = array_get(actual_elements_array,1);
        if(!element2)
            return;
        StaticMeshComponent* mesh2 = get_component_from_element(element2,STATIC_MESH_COMPONENT);
        unsigned int* modelid2 = array_get(&mesh2->meshes,1);

        Model* model1 = array_get(actual_model_array,*modelid);
        Model* model2 = array_get(actual_model_array,*modelid2);
    
        if( collision_of(model1,model2) ){
            LOG("collision SAT\n");
            
        }
        else{
            LOG("NO SAT\n");
        }
    }


}

void frame_clean(){
    //clean frame
    array_clean(&models_for_test_occlusion);
    array_clean(&array_static_meshes_pointers);
    array_clean(&array_static_meshes_pointers_for_test_distance);
    array_clean(&array_skinned_mesh_pointers);
    array_clean(&array_skinned_mesh_for_distance_test);
    for_each_element_components(&clean_component_value);
    //end clean frame
}

void editor_main_window_init(){
    window_create(window_editor_main, NULL, "Engine"); 

    glfwSetKeyCallback(window_editor_main->window, key_callback);
	glfwSetCursorPosCallback(window_editor_main->window, mouse_callback);
	glfwSetMouseButtonCallback(window_editor_main->window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_editor_main->window, window_resize_callback);
    glfwSetCharCallback(window_editor_main->window, character_callback);
    glfwSetWindowFocusCallback(window_editor_main->window,window_focus_callback);

    shader_compile_standard_shaders();

    draw_loading_screen();
    glfwSwapBuffers(window_editor_main->window);    
}

void editor_update(){

    editor_command_queue_udpate();    
    
    play_animation_list();

    //collision_test();    

}

void editor_render_init(){
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    init_vec3(-6,0,2, main_camera.position);
    camera_update(&current_window->camera);
    
    editor_standard_fragment_shader = compile_shader(editor_standard_fragment_shader_source, GL_FRAGMENT_SHADER);


    load_model_to_array(&engine_native_models,"../NativeContent/Editor/sphere.glb", "../NativeContent/Editor/sphere_diffuse.png");
    load_model_to_array(&engine_native_models,"../NativeContent/Editor/cube.glb", "../NativeContent/Editor/cube_diffuse.jpg");
    load_model_to_array(&engine_native_models,"../NativeContent/Editor/camera.gltf", "../NativeContent/Editor/camera_gizmo.jpg");
    load_model_to_array(&engine_native_models,"../NativeContent/Editor/floor.glb", "../NativeContent/Editor/floor.jpg");
    texture_load("../NativeContent/Editor/checker_texture.png",&editor_texture_checker);   
    gizmos_init();
    editor_running = true;
}

void editor_render_finish(){
	glfwTerminate();
}


void editor_draw(){

    glClearColor(COLOR(editor_background_color));
    render_clear_buffer(RENDER_COLOR_BUFFER | RENDER_DEPTH_BUFFER); 

   if(editor_content_browser_show){
	if(!editor_content_browser_initiliazed){
		editor_content_browser_initiliazed = true; 
		editor_window_content_get_models_path();    
		content_manager_init();
	}
		
	if(key_released(&input.A)){
		editor_content_browser_show = false;
		EditorWindow* level_editor_window = array_get(&editor_windows,0);
		level_editor_window->input = &editor_window_level_editor_input_update;
		return;
	}
		EditorWindow* level_editor_window = array_get(&editor_windows,0);
		level_editor_window->input = &editor_window_content_browser_input_update;
//		editor_window_content_browser_input_update();
		editor_window_content_browser_draw();
		text_render_in_screen_space(12,"test ofcouse",0,0);		
   	   return;	
   }	

	if(editor_file_explorer_show){
		file_explorer_update();
		if(key_released(&input.ESC)){
			editor_file_explorer_show = false;
			LOG("File explorer exit\n");	
		}	
		return;
	}

	if(isDrawUV)
        draw_UV();

    if(controlling_camera_component){
        CameraComponent* camera = get_component_from_selected_element(CAMERA_COMPONENT);
        update_main_camera_with_camera_component_values(camera);
    }

    for_each_element_components(&update_per_frame_component);
   
    test_elements_occlusion();
    check_meshes_distance();   

    editor_stats_draw_calls = frame_draw_elements.count;
	
	editor_stats_calculates_triangles();

    if(update_vertex_bones_gizmos)
        update_joints_vertex();

    engine_draw_elements(&frame_draw_elements);

    frame_clean();

    draw_gizmos();

    if(editor_mode == EDITOR_MODE_GUI_EDITOR || editor_mode == EDITOR_PLAY_MODE  ){
         draw_gui();         
    }       

    text_renderer_loop();
		
    //editor_message("editor message");    
}

void editor_main_render_thread(){

	window_manager_draw_windows();

}

void editor_init(){
	array_init(&editor_windows,sizeof(EditorWindow),40);
	
    actual_model_array = &editor_models;
    actual_elements_array = &editor_elements;
    current_textures_array = &editor_textures;

    array_init(&selected_elements_id,sizeof(unsigned short int),100);
    array_init(&LOD_models,sizeof(Model),10);
    array_init(&editor_elements,sizeof(Element),100);
    array_init(&editor_models, sizeof(Model),100);
    array_init(&editor_textures, sizeof(Texture),100);    
    
    editor_command_queue_init();   
    element_id_count = 0;    
    editor_mode = EDITOR_DEFAULT_MODE;
    editor_sub_mode = EDITOR_SUB_MODE_NULL;
    
    editor_mode_show_text = "Default Mode";
    editor_sub_mode_text = "";

    init_input();

    camera_velocity = 0.04;  

    edit_server_init();

	EditorWindow main_window;
	memset(&main_window,0,sizeof(EditorWindow));
	main_window.init = editor_main_window_init;
	main_window.draw = editor_draw;
	main_window.finish = editor_render_finish;
	main_window.input = editor_window_level_editor_input_update;
	array_add(&editor_windows,&main_window);
	window_editor_main = array_pop(&editor_windows);	


	//render thread initialization
    ExecuteCommand command;
    command.command = window_manager_init_window;
	command.parameter = window_editor_main;
    array_add(&array_render_thread_init_commmands,&command);

    engine_user_render_thread_init = editor_render_init;
    engine_user_render_thread_draw = editor_main_render_thread;
   	engine_user_render_thread_finish = editor_render_finish; 
	engine_client_render_thread_initialized = true;
	while(!window_editor_main->initialized){};

	window_manager_create_editor_windows_data();	
	LOG("[OK]Editor initialized\n");
}
