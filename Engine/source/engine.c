//
// Created by pavon on 6/24/19.
//

#include "engine.h"

#include "input.h"
#include "gui.h"
#include "Engine/shader.h"

#include "file_loader.h"

#include "images.h"

#include "model.h"

#include <unistd.h>

static inline void mvp_error(){
    LOG("MVP uniform not found\n");
    raise(SIGINT);
}

static inline void check_send_matrix_error(){
    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("[X] Send matrix error, Error %08x \n",error);
    }
}

void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(shader);   

    GLint mvp_uniform =  glGetUniformLocation(shader,"MVP");
    if(mvp_uniform == -1){
        GLint model_uniform = glGetUniformLocation(shader,"model");
        if(model_uniform == -1){
            mvp_error();
        }
        GLint projection_uniform = glGetUniformLocation(shader,"projection");
        GLint view_uniform = glGetUniformLocation(shader,"view");
        GLint joints_matrices_uniform = glGetUniformLocation(shader,"joint_matrix");

        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &matrix[0][0]);
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, &main_camera.projection[0][0]);
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &main_camera.view[0][0]);

        SkinnedMeshComponent* skin_component = get_component_from_selected_element(COMPONENT_SKINNED_MESH);
        glUniformMatrix4fv(joints_matrices_uniform,skin_component->node_uniform.joint_count , GL_FALSE, skin_component->node_uniform.joints_matrix);
  
        
        glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, joint));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, weight));
    }else{
        mat4 mvp;      
        update_mvp(matrix, mvp);  
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
    }    

    check_send_matrix_error();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);
}

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
    selected_element = get_from_array(actual_elements_array,actual_elements_array->count-1);
    selected_element->selected = true;
}

void new_empty_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));
        
    new_element.id = element_id_count;    

    element_id_count++;
    
    init_array(&new_element.components,sizeof(ComponentDefinition),6);

    add_to_array(actual_elements_array,&new_element);

    select_last_element();
}

void new_empty_model_in_array(Array* array){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    add_to_array(array,&new_model);

    selected_model = get_from_array(array,array->count-1);        
    
    selected_model->id = array->count-1;
}

void new_empty_model(){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    if(!actual_model_array)
        return;
    add_to_array(actual_model_array,&new_model);

    selected_model = get_from_array(actual_model_array,actual_model_array->count-1);        
    
    selected_model->id = actual_model_array->count-1;
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
    new_texture.image = load_image(image_path);
    load_model_texture_to_gpu(&new_texture); 
    
    add_to_array(current_textures_array,&new_texture);

    add_to_array(&textures_paths,image_path);
    
    Texture* texture_loaded = get_from_array(current_textures_array,current_textures_array->count-1);

    StaticMeshComponent* mesh = get_component_from_selected_element(STATIC_MESH_COMPONENT);
     
    if(mesh){
        if(mesh->meshes.count >= 1){
            int id = textures_paths.count-1;
            add_to_array(&mesh->textures,&id);
            for(int i = 1; i<mesh->meshes.count; i++){
                unsigned int* model_id = get_from_array(&mesh->meshes,i);
                Model* model = get_from_array(actual_model_array,*model_id);
                model->texture.id = texture_loaded->id;
                int id = textures_paths.count-1;
                add_to_array(&mesh->textures,&id);
            }
        }
        LOG("Texture loaded and assigned to Mesh Component: %s\n",image_path);
        return;
    }    
       
}

void load_simple_image(const char* path){
    Texture new_texture;
    memset(&new_texture,0,sizeof(Texture));
    new_texture.image = load_image(path);
    load_model_texture_to_gpu(&new_texture); 
    
    add_to_array(current_textures_array,&new_texture);    
}

int load_and_initialize_simple_model(const char* model_gltf_path){ 

    int load_model_result = load_model(model_gltf_path);
    if( load_model_result == -1){
        return -1;
    }
    if(load_model_result == 1){
        selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);

        init_model_gl_buffers(selected_model); 

    }
    return load_model_result; 
}

void add_element_with_model_path(const char* model_gltf_path){
    if(model_gltf_path == NULL){
        LOG("Error to load, null path (add_editor_element\n");
        return;
    }

    new_empty_element();
    strcpy(selected_element->name, "New Element");

    TransformComponent transform;
    init_transfrom_component(&transform);
    add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);

    add_to_array(&texts,model_gltf_path);

    Array* prev_array = actual_model_array;
    actual_model_array = &array_models_loaded;
    int models_loaded = load_and_initialize_simple_model(model_gltf_path);
    actual_model_array = prev_array;
    
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
            init_array(&mesh_component.meshes,sizeof(unsigned int),models_loaded+1);
            init_array(&mesh_component.textures,sizeof(unsigned int),models_loaded+1);
            int model_path_id = texts.count-1;
            add_to_array(&mesh_component.meshes,&model_path_id);

            //add model loaded id
            int id =        array_models_loaded.count-models_loaded;
            for(int i = 0; i<models_loaded ; i++){
                add_to_array(&mesh_component.meshes,&id);
                id++;
            }        

            add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);           
            
        }
    }

    for(int i = 0; i <selected_element->components.count ; i++){
        ComponentDefinition* component_definition = get_from_array(&selected_element->components,i);
        init_element_component(component_definition);
    }

    LOG("model loaded and shader created \n");
}


void update_viewport_size(){
    #ifdef EDITOR
    update_text_renderer_window_size();
    update_camera_aspect_ratio();
    #endif // EDITOR    
}

void set_selected_element_transform(vec3 position, versor rotation){
   
}

void draw_simgle_model(struct Model * new_model){
    glBindTexture(GL_TEXTURE_2D,new_model->texture.id);
    update_draw_vertices(new_model->shader,new_model->vertex_buffer_id,new_model->model_mat);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

    glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("draw error\n");
        LOG("Error %08x \n",error);
    }
}

void draw_elements(Array *elements){
    for(size_t i = 0; i < elements->count ; i++) { 
        Model** model = get_from_array(elements,i);
        
        draw_simgle_model(model[0]);
    }
    clean_array(elements);
    return;   
}


void load_model_texture_to_gpu(Texture* texture){
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, texture->image.width,
                    texture->image.heigth, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, texture->image.pixels_data);

    free_image(&texture->image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("texture error \n");
        LOG("Error %08x \n",error);
    }
}

void init_model_gl_buffers(struct Model* new_model){    

    Array vertex_array = new_model->vertex_array;

    glGenBuffers(1,&new_model->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof(struct Vertex) , vertex_array.data, GL_STATIC_DRAW);

    glGenBuffers(1,&new_model->index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    new_model->index_array.count * sizeof(unsigned short int),
                    new_model->index_array.data , GL_STATIC_DRAW);

    //free_to_marker(previous_marker); 
    //new_model->vertex_array.vertices = NULL;
    //new_model->index_array.data = NULL;
}

void set_element_position(Element* element, vec3 position){    
    glm_mat4_identity(element->transform->model_matrix);
    glm_translate(element->transform->model_matrix,position);
}

void compiles_standard_shaders(){
    standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
}

void init_engine(){
    init_array(&texts,sizeof(char[20]),50);
    init_array(&textures_paths,sizeof(char[20]),50);
    init_array(&array_models_loaded,sizeof(Model),100);

    init_camera();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    compiles_standard_shaders();

    init_gui();    

    init_array(&array_hirarchical_level_of_detail,sizeof(HierarchicalLevelOfDetail),5);
    
    action_pointer_id_count = 0;
    init_array(&actions_pointers,sizeof(ActionPointer),20);

    init_array(&frame_draw_elements,sizeof(void*),100);
    init_array(&models_for_test_occlusion,sizeof(void*),300);
    init_array(&array_static_meshes_pointers,sizeof(void*),300);
    init_array(&array_static_meshes_pointers_for_test_distance,sizeof(void*),100);
    init_array(&array_skinned_mesh_for_distance_test,sizeof(void*),100);
    init_array(&array_skinned_mesh_pointers,sizeof(void*),100);

    init_array(&array_animation_play_list,sizeof(Animation*),100);
    
    touch_position_x = -1;
    touch_position_x = -1;

    init_array(&engine_native_models,sizeof(Model),100);

    actual_standard_fragment_shader = standart_fragment_shader;    
    load_model_to_array(&engine_native_models,"editor/sphere.glb", "editor/sphere_diffuse.png");
    load_model_to_array(&engine_native_models,"editor/cube.glb", "editor/cube_diffuse.jpg");
    load_model_to_array(&engine_native_models,"editor/camera.gltf", "editor/camera_gizmo.jpg");
    load_model_to_array(&engine_native_models,"editor/floor.glb", "editor/floor.jpg");

}

Array models;
Array elements;
Array textures;

void init_game_engine(){
    should_close = false;    

    init_camera();        
    
    element_id_count = 0;   

    init_array(&models, sizeof(Model),100);
    init_array(&elements,sizeof(Element),100);
    init_array(&textures,sizeof(Texture),100);
    actual_model_array = &models;
    actual_elements_array = &elements;
    current_textures_array = &textures;

    actual_standard_fragment_shader = standart_fragment_shader;

    
}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for_each_element_components(&update_per_frame_component);
    test_elements_occlusion();

    draw_elements(&frame_draw_elements);
    clean_array(&models_for_test_occlusion);
    
    draw_gui();   


}

void add_action_function(void(*f)(void)){
    ActionPointer new_action;
    new_action.id = action_pointer_id_count;
    new_action.action = f;
    add_to_array(&actions_pointers,&new_action);
    action_pointer_id_count++;    
}

void load_model_to_array(Array* array, const char* path_model, const char* color_texture_path){   

    Array* prev_model_array = actual_model_array;
    actual_model_array = array;
        
    load_model(path_model);  

    selected_model->shader = create_engine_shader(standart_vertex_shader, standart_fragment_shader);

    glUseProgram(selected_model->shader);

    Texture new_texture;
    new_texture.image = load_image(color_texture_path);

    init_model_gl_buffers(selected_model);

    load_model_texture_to_gpu(&new_texture);
    selected_model->texture.id = new_texture.id;

    actual_model_array = prev_model_array;
}

void update_translation(vec3 translation){
    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(!transform)
        return;
    glm_translate(transform->model_matrix, translation);
    glm_vec3_copy(transform->model_matrix[3], transform->position);
    for(int i = 0; i<selected_element->components.count; i++){
        ComponentDefinition* component = get_from_array(&selected_element->components,i);
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

static inline void check_static_mesh_component_distance_from_camera(StaticMeshComponent* static_mesh_component){
    float distance = glm_vec3_distance(main_camera.position,static_mesh_component->center);
    Model* draw_model;
    float distaces[3] = {0,24,40};
    unsigned int * model_id;
    int count = static_mesh_component->meshes.count-1;
    for(int i = 1; i <= count ; i++){
        float distance_value = distaces[i-1];            
        if(distance_value==0){
            model_id = get_from_array(&static_mesh_component->meshes,i);
            continue;
        }
        float next_distace_value = distance_value+1;
        if(i != count)
            next_distace_value = distaces[i];
            
        if(distance >= distance_value && distance_value < next_distace_value){
            model_id = get_from_array(&static_mesh_component->meshes,i);
            continue;
        }
        
    }
    draw_model = get_from_array(actual_model_array,*model_id);
    
    add_to_array(&frame_draw_elements,&draw_model);
}  

void check_meshes_distance(){
    for(int i = 0; i < array_static_meshes_pointers_for_test_distance.count ; i++) { 
        StaticMeshComponent** 
        static_mesh_component_pointer_to_pointer = get_from_array(&array_static_meshes_pointers_for_test_distance,i);
        StaticMeshComponent* mesh_component = static_mesh_component_pointer_to_pointer[0];

        /*Simple LOD */
        check_static_mesh_component_distance_from_camera(mesh_component);

    }

    for(int i = 0; i < array_skinned_mesh_for_distance_test.count ; i++) { 
        SkinnedMeshComponent**
        skin_component_pointer_to_pointer = get_from_array(&array_skinned_mesh_for_distance_test,i);
        add_to_array(&frame_draw_elements,&skin_component_pointer_to_pointer[0]->mesh);
    }
    
}

void test_elements_occlusion(){
    vec4 frustrum_planes[6];
    mat4 view_projection_mat;
    glm_mat4_mul(main_camera.projection,main_camera.view,view_projection_mat);
    glm_frustum_planes(view_projection_mat,frustrum_planes);

    /*START: Deprecated in next update */
    for(size_t i = 0; i < models_for_test_occlusion.count ; i++) { 
        Model** model = get_from_array(&models_for_test_occlusion,i);
        Model* test_model = model[0];
        
        vec3 box[2];
        glm_vec3_copy(test_model->min,box[0]);
        glm_vec3_copy(test_model->max,box[1]);

        glm_aabb_transform(box, test_model->model_mat, box);

        if(glm_aabb_frustum(box,frustrum_planes) == true)
            add_to_array(&frame_draw_elements,&model[0]);
    }   
    /* END: Deprecated in next update */

    for(int i = 0; i<array_static_meshes_pointers.count; i++){
        StaticMeshComponent** static_mesh_component = get_from_array(&array_static_meshes_pointers,i);
        if(glm_aabb_frustum(static_mesh_component[0]->bounding_box,frustrum_planes) == true)
            add_to_array(&array_static_meshes_pointers_for_test_distance,&static_mesh_component[0]);
    }

    for(int i = 0; i<array_skinned_mesh_pointers.count; i++){
        SkinnedMeshComponent** skin_component = get_from_array(&array_skinned_mesh_pointers,i);
        if(glm_aabb_frustum(skin_component[0]->bounding_box,frustrum_planes) == true)
            add_to_array(&array_skinned_mesh_for_distance_test,&skin_component[0]);
    }

}

void duplicate_model_data(Model* destination , Model* source){
    memcpy(destination,source,sizeof(Model));
}