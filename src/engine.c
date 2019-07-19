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

    new_element.duplicated_of_id = -1;
    //new_element.model = get_from_array(actual_model_array,element_id_count);
    new_element.id = element_id_count;
    new_element.type = ELEMENT_TYPE_MODEL;

    glm_vec3_copy((vec3){0,0,0}, new_element.position);
    
    glm_quat_identity(new_element.rotation);    

    element_id_count++;
    
    init_array(&new_element.components,sizeof(ComponentDefinition),6);

    add_to_array(actual_elements_array,&new_element);

    select_last_element();
}
void new_empty_model(){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    add_to_array(actual_model_array,&new_model);

    selected_model = get_from_array(actual_model_array,actual_model_array->count-1);
        
    glm_mat4_identity(selected_model->model_mat);
    selected_model->id = model_id_count;
    model_id_count++;
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

    selected_element->model->texture.image = load_image(image_path);
    load_model_texture_to_gpu(selected_element->model);
    strcpy(selected_element->texture_path,image_path);
    
}

void add_element_with_model_path(const char* model_gltf_path){
    if(model_gltf_path == NULL){
        LOG("Error to load, null path (add_editor_element\n");
        return;
    }

    struct Model models[3];
    memset(models,0,sizeof(models));
    
    
    if( load_model(model_gltf_path,models ) == -1){
        return;
    }    

    new_empty_model();

    memcpy(&selected_model->index_array,&models[0].index_array,sizeof(Array));
    memcpy(&selected_model->vertex_array,&models[0].vertex_array,sizeof(Array));
    memcpy(&selected_model->min,&models[0].min,sizeof(vec3));
    memcpy(&selected_model->max,&models[0].max,sizeof(vec3));
    selected_model->skeletal = models[0].skeletal;

    struct Model* model0 = selected_model;     

    model0->shader = create_engine_shader(standart_vertex_shader,standart_fragment_shader);

    init_model_gl_buffers(model0);   
    
    new_empty_element();
    selected_element->model = selected_model;
    
    strcpy(selected_element->name, "New Element");
    TransformComponent transform;
    init_transfrom_component(&transform);
    add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);


    StaticMeshComponent mesh_component;
    mesh_component.model = selected_model;
    add_component_to_selected_element(sizeof(StaticMeshComponent),&mesh_component,STATIC_MESH_COMPONENT);

    strcpy(selected_element->model_path,model_gltf_path);

    LOG("model loaded and shader created \n");
}

void update_viewport_size(){
    #ifdef EDITOR
    update_text_renderer_window_size();
    update_camera_aspect_ratio();
    #endif // EDITOR    
}

void set_selected_element_transform(vec3 position, versor rotation){
    glm_translate(selected_element->model->model_mat, position);
    glm_vec3_add(selected_element->position,position,selected_element->position);

    mat4 model_rot_mat;
    glm_quat_mat4(rotation,model_rot_mat);

    glm_mul(selected_element->model->model_mat, model_rot_mat, selected_element->model->model_mat);

    glm_quat_copy(rotation, selected_element->rotation);
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
    
    GLfloat white[4] = {1, 1, 1, 1};
    GLfloat red[4] = {1, 0, 0, 1};

    for(size_t i = 0; i < elements->count ; i++) {                
        Element* element = get_from_array(elements,i);
        if(element->model == NULL)
            continue;
        if(element->model->id > model_id_count){
            LOG("Posible gargabe model pointer , model not draw\n ");
            continue;
        } 
        if(element->model->draw == false)
            continue;
        

        struct Model *new_model;
        struct Model *LOD0 = element->model;
        new_model = LOD0;     
        
        GLenum error;
        glBindTexture(GL_TEXTURE_2D, LOD0->texture.id);
        
        update_draw_vertices(new_model->shader,new_model->vertex_buffer_id,new_model->model_mat);

        
        GLint uniform_color =  glGetUniformLocation(new_model->shader,"color");
        if(element->selected)          
            glUniform4fv(uniform_color, 1, red);
        else
           glUniform4fv(uniform_color, 1, white);
        

        error = glGetError();
        if(error != GL_NO_ERROR){
            LOG("[X] Send unifrom COLOR error, Error %08x \n",error);

        }
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

        glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

        error = glGetError();
        if(error != GL_NO_ERROR){
            LOG("[X] Draw elements, Error %08x \n",error);
        }
    }

}


void load_model_texture_to_gpu(struct Model * model){
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

    init_camera();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    compiles_standard_shaders();

    init_gui();    

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

    init_array(&frame_draw_elements,sizeof(void*),300);

}
Array models;
Array elements;

void init_game_engine(){
    should_close = false;    

    init_camera();        
    
    element_id_count = 0;   

    init_array(&models, sizeof(Model),100);
    init_array(&elements,sizeof(Element),100);
    actual_model_array = &models;
    actual_elements_array = &elements;
    //actual_LOD_models_array = &LOD_models;
    actual_standard_fragment_shader = standart_fragment_shader;

    
}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    draw_elements(actual_elements_array); 
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

    new_model.texture.image = load_image(color_texture_path);

    init_model_gl_buffers(&new_model);

    load_model_texture_to_gpu(&new_model);

    add_to_array(array,&new_model);  

}

void update_translation(vec3 translation){
    //glm_translate(selected_element->model->model_mat, translation);
    glm_vec3_add(selected_element->position,translation,selected_element->position);
    if(selected_element->transform != NULL){
        glm_translate(selected_element->transform->model_matrix, translation);
    }
}