//
// Created by pavon on 6/24/19.
//

#include "engine.h"

#include "gui.h"
#include "shader.h"

#include "file_loader.h"

#include "images.h"

#include "model.h"

#include <unistd.h>

#include "LOD_system.h"


Array engine_models;
Array engine_elements;
Array engine_textures;
void init_static_gpu_vertex_buffer(Array* array, GLuint *id){
    glGenBuffers(1,id);
    GLuint id_copy;
    memcpy(&id_copy,id,sizeof(GLuint));
    glBindBuffer(GL_ARRAY_BUFFER,id_copy);
    glBufferData(GL_ARRAY_BUFFER, array->count * sizeof(struct Vertex) , array->data, GL_STATIC_DRAW);

}

void init_static_gpu_index_buffer(Array* array, GLuint *id){
    glGenBuffers(1,id);
    GLuint id_copy;
    memcpy(&id_copy,id,sizeof(GLuint));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id_copy);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array->count * sizeof(struct Vertex) , array->data, GL_STATIC_DRAW);
}

void update_gpu_vertex_data(Array* array, GLuint id){
    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER, array->count * sizeof(struct Vertex) , array->data, GL_STATIC_DRAW);

}

void select_last_element(){
    if(selected_element != NULL)
        selected_element->selected = false;
    selected_element = array_get(actual_elements_array,actual_elements_array->count-1);
    selected_element->selected = true;
}

void new_empty_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));
        
    new_element.id = element_id_count;   
    new_element.proccess = true; 

    element_id_count++;
    
    array_init(&new_element.components,sizeof(ComponentDefinition),6);

    array_add(actual_elements_array,&new_element);

    select_last_element();
}

void new_empty_model_in_array(Array* array){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    array_add(array,&new_model);

    selected_model = array_get(array,array->count-1);        
    
    selected_model->id = array->count-1;
}

void new_empty_model(){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    if(!actual_model_array)
        return;
    glm_mat4_identity(new_model.model_mat);
    array_add(actual_model_array,&new_model);

    selected_model = array_get(actual_model_array,actual_model_array->count-1);        
    
    selected_model->id = actual_model_array->count-1;

}


void engine_select_element_add_texture(Texture* texture){

    array_add(current_textures_array,&texture);
    Texture* texture_loaded = array_get(current_textures_array,current_textures_array->count-1);

    StaticMeshComponent* mesh = get_component_from_selected_element(STATIC_MESH_COMPONENT);
     
    if(mesh){
        if(mesh->meshes.count >= 1){
            int id = textures_paths.count-1;
            array_add(&mesh->textures,&id);
            for(int i = 1; i<mesh->meshes.count; i++){
                u8* model_id = array_get(&mesh->meshes,i);
                Model* model = array_get(actual_model_array,*model_id);
                model->texture.id = texture_loaded->id;
                u8 id = textures_paths.count-1;
                array_add(&mesh->textures,&id);
            }
        }
        return;
    }    
    
    SkinnedMeshComponent* skin_component = get_component_from_selected_element(COMPONENT_SKINNED_MESH);
    int id = textures_paths.count-1;
    Texture* last_texturer = array_get(current_textures_array,current_textures_array->count-1);
    skin_component->mesh->texture.id = last_texturer->id;
}
void engine_add_texture_from_memory_to_selected_element(void* data, u32 size){
	Texture new_texture;
	texture_load_from_memory(&new_texture,size,data);	
    array_add(&textures_paths,"from_memory");
	engine_select_element_add_texture(&new_texture);
}

void add_texture_to_selected_element_with_image_path(const char* image_path){

    if(selected_element == NULL){
        LOG("No element selected\n"); 
        return;
    }
    if(image_path == NULL){
        LOG("Error to load, null path (add_editor_texture - 154\n");
        return;
    }
    
    Texture new_texture;
    memset(&new_texture,0,sizeof(Texture));
    texture_load(image_path,&new_texture);

    array_add(&textures_paths,image_path);
    engine_select_element_add_texture(&new_texture);
	LOG("Texture loaded and assigned to Mesh Component: %s\n",image_path);
}

void load_simple_image(const char* path){
    Texture new_texture;
    texture_load(path,&new_texture);
       
    array_add(current_textures_array,&new_texture);    
}

void engine_add_element(u32 models_loaded){
    new_empty_element();   

    add_transform_component_to_selected_element();

    switch (current_loaded_component_type)
    {
    case COMPONENT_SKINNED_MESH:
        {
            LOG("Skinned mesh need fill now\n");
            SkinnedMeshComponent skin_mesh_component;
            memset(&skin_mesh_component,0,sizeof(SkinnedMeshComponent));
            
            add_component_to_selected_element(sizeof(SkinnedMeshComponent),&skin_mesh_component,COMPONENT_SKINNED_MESH);

        }
        break;
    
    case STATIC_MESH_COMPONENT:
        {
            StaticMeshComponent mesh_component;
            memset(&mesh_component,0,sizeof(StaticMeshComponent));    
            array_init(&mesh_component.meshes,sizeof(u8),models_loaded+1);//the first element is the id of the model path in texts.array
            array_init(&mesh_component.textures,sizeof(u8),models_loaded+1);

            u8 model_path_id = texts.count-1;
            array_add(&mesh_component.meshes,&model_path_id);

            //add model loaded id
            u8 id = array_models_loaded.count-models_loaded;
            for(u8 i = 0; i<models_loaded ; i++){
                array_add(&mesh_component.meshes,&id);
                id++;
            }        

            add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);           
            
        }
    }

    for(int i = 0; i <selected_element->components.count ; i++){
        ComponentDefinition* component_definition = array_get(&selected_element->components,i);
        init_element_component(component_definition);
    }

    LOG("model loaded and shader created \n");
}

void engine_add_element_from_content(Content* content){
    
}

void add_element_with_model_path(const char* model_gltf_path){
    if(model_gltf_path == NULL || model_gltf_path[0] == '\0'){
        LOG("Error to load, null path (add_editor_element)\n");
        return;
    }

    Array* prev_array = actual_model_array;
    actual_model_array = &array_models_loaded;

    int models_loaded = load_model(model_gltf_path);
    if( models_loaded == -1){
        return;
    }
    actual_model_array = prev_array;
    
    array_add(&texts,model_gltf_path);
    
    engine_add_element(models_loaded);
}


void update_viewport_size(){
    #ifdef EDITOR
    text_renderer_update_pixel_size();
    update_camera_aspect_ratio();
    #endif // EDITOR    
}

void set_selected_element_transform(vec3 position, versor rotation){
   
}



void draw_elements(Array *elements){
    for(size_t i = 0; i < elements->count ; i++) { 
        Model** model = array_get(elements,i);
        Model* draw_model = model[0];        
        draw_simgle_model(draw_model);
    }
    array_clean(elements);
}

void set_element_position(Element* element, vec3 position){    
    glm_mat4_identity(element->transform->model_matrix);
    glm_translate(element->transform->model_matrix,position);
}
void engine_client_initialize_render_thread(){
    if(array_render_thread_init_commmands.count == 0){
        LOG("Critical, no render thread initialize commmand\n");
        debug_break();
    }

    for (u8 i = 0; i < array_render_thread_init_commmands.count; i++)
    {
        ExecuteCommand* exectute = array_get(&array_render_thread_init_commmands,i);
        exectute->command(NULL);
    }
	array_clean(&array_render_thread_init_commmands);

    engine_user_render_thread_init();
    engine_initialized = true;
}

void engine_render_thread_init(){

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);   
    
    init_camera();  
    init_gui();
	text_renderer_init();
}

bool engine_user_render_thread_initialized_in_loop = false;

void engine_render_thread(){
   
	float render_frame_time = 0;
    float disired_frame_time = 0.016f;
    
    u8 frames = 0;
    float frame_second = 0;
    while (engine_running)
    {
		if(!engine_user_render_thread_initialized_in_loop){

			   if(engine_client_render_thread_initialized){
					engine_client_initialize_render_thread(); 
					engine_render_thread_init();
					engine_user_render_thread_initialized_in_loop = true;
			   } 
	}

	render_frame_time += time_delta;
        
        time_start();  
		int executed_commmand_count = 0;	
		for(u8 i = 0; i<array_render_thread_commands.count; i++){
			ExecuteCommand* command = array_get(&array_render_thread_commands,i);
			if(command->executed == false){
			command->command(NULL);
			command->executed = true;
			executed_commmand_count++;
			}
		}
		if(executed_commmand_count==0){
			array_clean(&array_render_thread_commands);
		}
		if(engine_client_render_thread_initialized)
	        engine_user_render_thread_draw();
	
        time_end();
        frame_second += time_elapsed_time;
        if(frame_second >= 1000){
            FPS = frames * (1000.f / frame_second);
            frames = 0;
            frame_second = 0;
        }else
            frames++;
    }
	engine_user_render_thread_finish();    
}

void engine_init_render(){
    thread_new_detached(engine_render_thread,NULL,"Render");    
}

void engine_init_data(){
    array_init(&texts,sizeof(char[100]),50);
    array_init(&textures_paths,sizeof(char[20]),50);
    array_init(&array_models_loaded,sizeof(Model),100);

    array_init(&engine_native_models,sizeof(Model),100);   

    array_init(&array_hirarchical_level_of_detail,sizeof(HierarchicalLevelOfDetail),5);
        
    array_init(&actions_pointers,sizeof(ActionPointer),20);

    array_init(&frame_draw_elements,sizeof(void*),100);
    array_init(&models_for_test_occlusion,sizeof(void*),300);
    array_init(&array_static_meshes_pointers,sizeof(void*),300);
    array_init(&array_static_meshes_pointers_for_test_distance,sizeof(void*),100);
    array_init(&array_skinned_mesh_for_distance_test,sizeof(void*),100);
    array_init(&array_skinned_mesh_pointers,sizeof(void*),100);

    array_init(&array_animation_play_list,sizeof(Animation*),100);
    
    touch_position_x = -1;
    touch_position_x = -1;

    action_pointer_id_count = 0;    

    actual_standard_fragment_shader = standart_fragment_shader;  
}

void engine_init(){
    array_init(&array_render_thread_init_commmands,sizeof(ExecuteCommand),5);
	array_init(&array_render_thread_commands,sizeof(ExecuteCommand),100);
   
    engine_init_data();   
    engine_running = true;
}


void init_game_engine(){
    should_close = false;    

    init_camera();        
    
    element_id_count = 0;   

    array_init(&engine_models, sizeof(Model),100);
    array_init(&engine_elements,sizeof(Element),100);
    array_init(&engine_textures,sizeof(Texture),100);
    actual_model_array = &engine_models;
    actual_elements_array = &engine_elements;
    current_textures_array = &engine_textures;

    actual_standard_fragment_shader = standart_fragment_shader;    
}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for_each_element_components(&update_per_frame_component);
    test_elements_occlusion();

    draw_elements(&frame_draw_elements);
    array_clean(&models_for_test_occlusion);
    
    draw_gui();   
}

void add_action_function(void(*f)(void)){
    ActionPointer new_action;
    new_action.id = action_pointer_id_count;
    new_action.action = f;
    array_add(&actions_pointers,&new_action);
    action_pointer_id_count++;    
}

void load_model_to_array(Array* array, const char* path_model, const char* color_texture_path){   

    Array* prev_model_array = actual_model_array;
    actual_model_array = array;
        
    load_model(path_model);


    selected_model->shader = create_engine_shader(standart_vertex_shader, standart_fragment_shader);

    glUseProgram(selected_model->shader);

    Texture new_texture;
    texture_load(color_texture_path,&new_texture);

    GPU_buffers_create_for_model(selected_model);

    selected_model->texture.id = new_texture.id;

    actual_model_array = prev_model_array;
}

void update_translation(vec3 translation){
    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(!transform)
        return;
    vec3 translation_per_frame;
    glm_vec3_scale(translation,time_delta,translation_per_frame);
    glm_translate(transform->model_matrix, translation_per_frame);
    glm_vec3_copy(transform->model_matrix[3], transform->position);
    for(int i = 0; i<selected_element->components.count; i++){
        ComponentDefinition* component = array_get(&selected_element->components,i);
        update_component(component);
    }
}

void update_scale(vec3 translation){
    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(!transform)
        return;

    glm_vec3_add(transform->scale,translation,transform->scale);
    vec3 identity;
    glm_vec3_copy(GLM_VEC3_ONE,identity);
    glm_vec3_add(identity,translation,translation);

    glm_scale(transform->model_matrix,translation);
    
    for(int i = 0; i<selected_element->components.count; i++){
        ComponentDefinition* component = array_get(&selected_element->components,i);
        update_component(component);
    }
}

void rotate_element(Element* element, versor quaternion){
    mat4 model_rot_mat;
    glm_quat_mat4(quaternion,model_rot_mat);

    TransformComponent* transform = get_component_from_element(element,TRASNFORM_COMPONENT);
    if(transform)
        glm_mul(transform->model_matrix,model_rot_mat, transform->model_matrix);

}


void check_meshes_distance(){
    for(int i = 0; i < array_static_meshes_pointers_for_test_distance.count ; i++) { 
        StaticMeshComponent** ppStaticMesh = array_get(&array_static_meshes_pointers_for_test_distance,i);
        StaticMeshComponent* mesh_component = ppStaticMesh[0];

        /*Simple LOD */
        LOD_check_distance_static_mesh_component_and_add_to_draw_elements(mesh_component);

    }

    for(int i = 0; i < array_skinned_mesh_for_distance_test.count ; i++) { 
        SkinnedMeshComponent** ppSkinComponent = array_get(&array_skinned_mesh_for_distance_test,i);
        array_add(&frame_draw_elements,&ppSkinComponent[0]->mesh);
    }
    
}

void test_elements_occlusion(){
    vec4 frustrum_planes[6];
    mat4 view_projection_mat;
    glm_mat4_mul(main_camera.projection,main_camera.view,view_projection_mat);
    glm_frustum_planes(view_projection_mat,frustrum_planes);


    for(size_t i = 0; i < models_for_test_occlusion.count ; i++) { 
        Model** model = array_get(&models_for_test_occlusion,i);
        Model* test_model = model[0];
        
        vec3 box[2];
        glm_vec3_copy(test_model->min,box[0]);
        glm_vec3_copy(test_model->max,box[1]);

        glm_aabb_transform(box, test_model->model_mat, box);

        if(glm_aabb_frustum(box,frustrum_planes) == true)
            array_add(&frame_draw_elements,&model[0]);
    }   


    for(int i = 0; i<array_static_meshes_pointers.count; i++){
        StaticMeshComponent** static_mesh_component = array_get(&array_static_meshes_pointers,i);
        if(glm_aabb_frustum(static_mesh_component[0]->bounding_box,frustrum_planes) == true)
            array_add(&array_static_meshes_pointers_for_test_distance,&static_mesh_component[0]);
    }

    for(int i = 0; i<array_skinned_mesh_pointers.count; i++){
        SkinnedMeshComponent** skin_component = array_get(&array_skinned_mesh_pointers,i);
        if(glm_aabb_frustum(skin_component[0]->bounding_box,frustrum_planes) == true)
            array_add(&array_skinned_mesh_for_distance_test,&skin_component[0]);
    }

}

void duplicate_model_data(Model* destination , Model* source){
    memcpy(destination,source,sizeof(Model));
}
