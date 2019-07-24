//
// Created by pavon on 6/24/19.
//

#include "engine.h"

#include "input.h"
#include "gui.h"
#include "shader.h"

#include "file_loader.h"

#include "images.h"

#include "model.h"

#include <unistd.h>

void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(shader);

    mat4 mvp;      
    update_mvp(matrix, mvp);  

    GLint mvp_uniform =  glGetUniformLocation(shader,"MVP");
    if(mvp_uniform == -1){
        LOG("MVP uniform not found\n");
        raise(SIGINT);
        exit(-1);
        return;
    }

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
    
    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("[X] Send matrix error, Error %08x \n",error);
    }
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
    new_element.type = ELEMENT_TYPE_MODEL;

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
            for(int i = 0; i<mesh->meshes.count; i++){
                unsigned int* model_id = get_from_array(&mesh->meshes,i);
                Model* model = get_from_array(actual_model_array,*model_id);
                model->texture.id = texture_loaded->id;
                int id = textures_paths.count-1;
                add_to_array(&mesh->textures,&id);
            }
        }else{
            mesh->texture_id = textures_paths.count-1;
            mesh->model->texture.id = texture_loaded->id;//for compatibility
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
    new_empty_model();
    int load_model_result = load_model(model_gltf_path,selected_model);
    if( load_model_result == -1){
        return -1;
    }
    if(load_model_result == 0){
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


    int models_loaded = load_and_initialize_simple_model(model_gltf_path); 
    
    StaticMeshComponent mesh_component;
    if(models_loaded == 0){
        
        mesh_component.model = selected_model;
        
        mesh_component.model_id = texts.count-1;
        
    }
    if(models_loaded > 1){
        init_array(&mesh_component.meshes,sizeof(unsigned int),models_loaded);
        init_array(&mesh_component.textures,sizeof(unsigned int),models_loaded);
        int model_id_actual_model_array = actual_model_array->count - models_loaded;        
        for(int i = 0; i<models_loaded ; i++){
            new_empty_model_in_array(actual_model_array);
            Model* original_model = get_from_array(actual_model_array,model_id_actual_model_array);
            duplicate_model_data(selected_model,original_model);
            selected_model->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
            int new_model_id = actual_model_array->count -1;
            add_to_array(&mesh_component.meshes,&new_model_id);
            model_id_actual_model_array++;
        }
    }

    add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);
    add_to_array(&texts,model_gltf_path);
        
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
void load_models_texture_to_gpu(Array* models_array){
    for(size_t i = 0; i < models_array->count ; i++) {
        struct Model *model = get_from_array(&models_array,i);


        glGenTextures(1, &model->texture.id);
        glBindTexture(GL_TEXTURE_2D, model->texture.id);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, model->texture.image.width,
                      model->texture.image.heigth, 0,
                      GL_RGB, GL_UNSIGNED_BYTE, model->texture.image.pixels_data);

        free_image(&model->texture.image);

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

    init_camera();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    compiles_standard_shaders();

    init_gui();    

    init_array(&array_hirarchical_level_of_detail,sizeof(HirarchicalLevelOfDetail),5);
    init_array(&components,sizeof(ComponentDefinition),100);
    components_id_count = 0;

    Array test_numbers;
    init_array(&test_numbers,sizeof(unsigned short int), 3);
    unsigned short int a = 1;
    unsigned short int b = 2;
    unsigned short int c = 3;
    add_to_array(&test_numbers,&a);
    add_to_array(&test_numbers,&b);
    add_to_array(&test_numbers,&c);

    for (size_t i = 0; i < test_numbers.count; i++)
    {
        int* element = get_from_array(&test_numbers,i);
        int number;
        memcpy(&number,element,sizeof(unsigned short int));
        LOG("Number: %i\n",number);
    }


    action_pointer_id_count = 0;
    init_array(&actions_pointers,sizeof(ActionPointer),20);

    init_array(&frame_draw_elements,sizeof(void*),100);
    init_array(&models_for_test_occlusion,sizeof(void*),300);

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

void clean_elements_components(){
    for(int i = 0; i < actual_elements_array->count ; i++){
        Element* element = get_from_array(actual_elements_array,i);
        if(element->components_count > 0){
            for(int o = 0; o < element->components_count ; o++){
                ComponentDefinition* component = get_from_array(&element->components,o);
                clean_component_value(component);
            }
        }
    }
}

void update_elements_components(){
    for(int i = 0; i < actual_elements_array->count ; i++){
        Element* element = get_from_array(actual_elements_array,i);
        if(element->components_count > 0){
            for(int o = 0; o < element->components_count ; o++){
                ComponentDefinition* component = get_from_array(&element->components,o);
                update_component(component);
            }
        }
    }
}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    update_elements_components();
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

    struct Model new_model;
    memset(&new_model,0,sizeof(Model));
    
    load_model(path_model,&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = create_engine_shader(standart_vertex_shader, standart_fragment_shader);

    glUseProgram(new_model.shader);

    Texture new_texture;
    new_texture.image = load_image(color_texture_path);

    init_model_gl_buffers(&new_model);

    load_model_texture_to_gpu(&new_texture);
    new_model.texture.id = new_texture.id;

    add_to_array(array,&new_model);  

}

void update_translation(vec3 translation){
    TransformComponent* transform = get_component_from_selected_element(TRASNFORM_COMPONENT);
    if(!transform)
        return;
    glm_translate(transform->model_matrix, translation);
    glm_vec3_copy(VEC3(transform->model_matrix[3][0],
                        transform->model_matrix[3][1],
                        transform->model_matrix[3][2]),
    transform->position);
}

void rotate_element(Element* element, versor quaternion){
    mat4 model_rot_mat;
    glm_quat_mat4(quaternion,model_rot_mat);

    TransformComponent* transform = get_component_from_element(element,TRASNFORM_COMPONENT);
    if(transform)
        glm_mul(transform->model_matrix,model_rot_mat, transform->model_matrix);

}

void test_elements_occlusion(){
    vec4 frustrum_planes[6];
    mat4 view_projection_mat;
    glm_mat4_mul(main_camera.projection,main_camera.view,view_projection_mat);

    glm_frustum_planes(view_projection_mat,frustrum_planes);



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
}

void duplicate_model_data(Model* destination , Model* source){
    destination->vertex_array.count = source->vertex_array.count;
    destination->index_array.count = source->index_array.count;
    destination->index_buffer_id = source->index_buffer_id;
    destination->vertex_buffer_id = source->vertex_buffer_id;
    destination->texture.id = source->texture.id;
    glm_vec3_copy(source->min,destination->min);
    glm_vec3_copy(source->max,destination->max);
}