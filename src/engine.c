//
// Created by pavon on 6/24/19.
//

#include "engine.h"

#include "input.h"
#include "gui.h"
#include "shader.h"

#include "file_loader.h"

#include "images.h"

#include "vector.h"
#include "model.h"

#include <unistd.h>



void update_draw_vertices(GLuint shader, GLuint buffer){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glUseProgram(shader);

    mat4 mvp;
    glm_mat4_identity(mvp);
    
    
    mat4 model;
    glm_mat4_identity(model);
    update_mvp(model, mvp);
  

    GLint mvp_uniform =  glGetUniformLocation(shader,"MVP");
    if(mvp_uniform == -1){
        printf("MVP uniform not found\n");
        return;
    }

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
    
    GLenum error;
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("[X] Send matrix error, Error %08x \n",error);
    }
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);
}

void init_static_gpu_vertex_buffer(VertexArray* array, GLuint *id){
    glGenBuffers(1,id);
    GLuint id_copy;
    memcpy(&id_copy,id,sizeof(GLuint));
    glBindBuffer(GL_ARRAY_BUFFER,id_copy);
    glBufferData(GL_ARRAY_BUFFER, array->count * sizeof(struct Vertex) , array->vertices, GL_STATIC_DRAW);

}

void update_gpu_vertex_data(VertexArray* array, GLuint id){
    glBindBuffer(GL_ARRAY_BUFFER,id);
    glBufferData(GL_ARRAY_BUFFER, array->count * sizeof(struct Vertex) , array->vertices, GL_STATIC_DRAW);

}

void select_last_element(){
    if(selected_element != NULL)
        selected_element->selected = false;
    selected_element = get_element_from_array(actual_elements_array,actual_elements_array->count-1);
    selected_element->selected = true;
}

void new_empty_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));

    new_element.duplicated_of_id = -1;
    new_element.model = get_element_from_array(actual_model_array,element_id_count);
    new_element.id = element_id_count;
    new_element.type = ELEMENT_TYPE_MODEL;

    glm_vec3_copy((vec3){0,0,0}, new_element.position);
    
    glm_quat_identity(new_element.rotation);    

    element_id_count++;
    
    init_array_with_count(&new_element.components,sizeof(ComponentDefinition),6);

    add_element_to_array(actual_elements_array,&new_element);

    select_last_element();
}
void new_empty_model(){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    add_element_to_array(actual_model_array,&new_model);

    selected_model = get_element_from_array(actual_model_array,actual_model_array->count-1);
        
    glm_mat4_identity(selected_model->model_mat);
    selected_model->id = model_id_count;
    model_id_count++;
}

void add_texture_to_selected_element_with_image_path(const char* image_path){

    if(selected_element == NULL){
        printf("No element selected\n"); 
        return;
    }
    if(image_path == NULL){
        printf("Error to load, null path (add_editor_texture - 154\n");
        return;
    }

    selected_element->model->texture.image = load_image(image_path);
    load_model_texture_to_gpu(selected_element->model);
    strcpy(selected_element->texture_path,image_path);
    
}

void add_element_with_model_path(const char* model_gltf_path){
    if(model_gltf_path == NULL){
        printf("Error to load, null path (add_editor_element\n");
        return;
    }

    struct Model models[3];
    memset(models,0,sizeof(models));

    
    int result = load_model(model_gltf_path,models);
    if(result==-1){
        return;
    }    
    
    new_empty_model();
    memcpy(&selected_model->index_array,&models[0].index_array,sizeof(IndexArray));
    memcpy(&selected_model->vertex_array,&models[0].vertex_array,sizeof(VertexArray));
    memcpy(&selected_model->min,&models[0].min,sizeof(vec3));
    memcpy(&selected_model->max,&models[0].max,sizeof(vec3));
    selected_model->skeletal = models[0].skeletal;

    struct Model* model0 = selected_model;     

    model0->shader = create_engine_shader(standart_vertex_shader,actual_standard_fragment_shader);

    init_model(model0);
    model0->actual_LOD = 0;
    model0->change_LOD = false;
    model0->draw = true;
    

    if(models[0].LOD_count >= 1){
        for(int i = 0; i < models[0].LOD_count; i++){
            glm_mat4_identity(models[i+1].model_mat);  
            init_model(&models[i+1]);
            models[i+1].shader = model0->shader;
            add_element_to_array(actual_LOD_models_array,&models[i+1]);
        }
    }
    if(models[0].has_HLOD){
        glm_mat4_identity(models[2].model_mat);  
        init_model(&models[2]);
        models[2].shader = model0->shader;
        add_element_to_array(actual_LOD_models_array,&models[2]);
        models->HLOD = get_element_from_array(actual_LOD_models_array,actual_LOD_models_array->count-1);
        
    }    
    
    new_empty_element();
    selected_element->model = selected_model;
    selected_element->model_id = selected_model->id;

    strcpy(selected_element->model_path,model_gltf_path);
    
    if(model0->LOD_count >= 1){
        selected_element->has_LOD  = true;
    }  

    printf("model loaded and shader created \n");
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
    GLenum error ;
    glUseProgram(new_model->shader);
    glBindTexture(GL_TEXTURE_2D, new_model->texture.id);
    
    mat4 mvp;
    glm_mat4_identity(mvp);
    
    update_mvp(new_model->model_mat, mvp);

    GLint mvp_uniform =  glGetUniformLocation(new_model->shader,"MVP");

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
    
    error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("draw error\n");
        LOGW("Error %08x \n",error);
    }

    glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);


    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

    glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

    error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("draw error\n");
        LOGW("Error %08x \n",error);
    }
}

void draw_elements(Array *elements){
    GLfloat white[4] = {1, 1, 1, 1};
    GLfloat red[4] = {1, 0, 0, 1};

    for(size_t i = 0; i < elements->count ; i++) {                
        Element* element = get_element_from_array(elements,i);
        if(element->model == NULL)
            continue;
        if(element->model->draw == false)
            continue;
        if(element->model->id > model_id_count){
            printf("Posible gargabe model pointer , model not draw\n ");
            continue;
        } 

        struct Model *new_model;
        struct Model *LOD0 = element->model;
        new_model = LOD0;
        if(new_model->change_LOD){
            new_model = new_model->LOD;          
        }
        if(LOD0->change_to_HLOD){
            //new_model = LOD0->HLOD;
        }
        
        GLenum error;
        
        glBindBuffer(GL_ARRAY_BUFFER, new_model->vertex_buffer_id);

        glUseProgram(new_model->shader);

        mat4 mvp;
        glm_mat4_identity(mvp);
        
        update_mvp(new_model->model_mat, mvp);


        GLint mvp_uniform =  glGetUniformLocation(new_model->shader,"MVP");
        if(mvp_uniform == -1){
            printf("MVP uniform not found\n");
            return;
        }

        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

        error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("[X] Send matrix error, Error %08x \n",error);
        }
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glBindTexture(GL_TEXTURE_2D, LOD0->texture.id);
        GLint uniform_color =  glGetUniformLocation(new_model->shader,"color");
        if(element->selected)          
            glUniform4fv(uniform_color, 1, red);
        else
           glUniform4fv(uniform_color, 1, white);
        

        error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("[X] Send unifrom color error, Error %08x \n",error);
        }

        glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);    

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

        glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

        error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("[X] Draw elements, Error %08x \n",error);
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
        LOGW("texture error \n");
        LOGW("Error %08x \n",error);
    }
}
void load_models_texture_to_gpu(Array* models_array){
    for(size_t i = 0; i < models_array->count ; i++) {
        struct Model *model = get_element_from_array(&models_array,i);


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
            LOGW("texture error \n");
            LOGW("Error %08x \n",error);
        }

    }
}

void init_model(struct Model* new_model){    

    VertexArray vertex_array = new_model->vertex_array;


    glGenBuffers(1,&new_model->vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof(struct Vertex) , vertex_array.vertices, GL_STATIC_DRAW);


    glGenBuffers(1,&new_model->index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    new_model->index_array.count * sizeof(unsigned short int),
                    new_model->index_array.indices , GL_STATIC_DRAW);

    //free(new_model->vertex_array.vertices);
    free_to_marker(previous_marker);
    //free(new_model->index_array.indices);
    new_model->vertex_array.vertices = NULL;
    new_model->index_array.indices = NULL;
}

void init_models(Array* array){
    for(size_t i = 0; i < array->count ; i++){

        struct Model* new_model = get_element_from_array(array,i);

        VertexArray vertex_array = new_model->vertex_array;


        glGenBuffers(1,&new_model->vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof(struct Vertex) , vertex_array.vertices, GL_STATIC_DRAW);


        glGenBuffers(1,&new_model->index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     new_model->index_array.count * sizeof(unsigned short int),
                     new_model->index_array.indices , GL_STATIC_DRAW);

        free(new_model->vertex_array.vertices);
        //free(new_model->index_array.indices);
    }

}

void set_element_position(Element* element, vec3 position){
    glm_mat4_identity(element->model->model_mat);
    glm_translate(element->model->model_mat,position);
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

    init_array_with_count(&components,sizeof(ComponentDefinition),100);
    components_id_count = 0;

    Array test_numbers;
    init_array_with_count(&test_numbers,sizeof(int), 3);
    int a = 1;
    int b = 2;
    int c = 3;
    add_element_to_array(&test_numbers,&a);
    add_element_to_array(&test_numbers,&b);
    //add_element_to_array(&test_numbers,&c);

    for (size_t i = 0; i < test_numbers.count; i++)
    {
        int* element = get_element_from_array(&test_numbers,i);
        int number;
        memcpy(&number,element,sizeof(int));
        printf("Number: %i\n",number);
    }


    action_pointer_id_count = 0;
    init_array_with_count(&actions_pointers,sizeof(ActionPointer),20);

}
Array models;
Array elements;

void init_game_engine(){
    should_close = false;    

    init_camera();        
    
    element_id_count = 0;   

    init_array_with_count(&models, sizeof(Model),100);
    init_array_with_count(&elements,sizeof(Element),100);
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
    add_element_to_array(&actions_pointers,&new_action);
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

    init_model(&new_model);

    load_model_texture_to_gpu(&new_model);

    add_element_to_array(array,&new_model);  

}

void update_translation(vec3 translation){
    glm_translate(selected_element->model->model_mat, translation);
    glm_vec3_add(selected_element->position,translation,selected_element->position);
    if(selected_element->transform != NULL){
        glm_translate(selected_element->transform->model_matrix, translation);
    }
}