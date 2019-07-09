//
// Created by pavon on 6/24/19.
//

#include "engine.h"

#include "input.h"
#include "gui.h"
#include "shader.h"

#include "game.h"

#include "file_loader.h"

#include "images.h"

#include "vector.h"
#include "model.h"
#include "level.h"


#include <unistd.h>



void draw_frame(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {

        struct Model *new_model = &new_level.models_array.models[i];

        glUseProgram(new_model->shader);
        glBindTexture(GL_TEXTURE_2D, new_model->texture.id);
        
        
        mat4 mvp;
        glm_mat4_identity(mvp);

        update_mvp(new_model->model_mat, mvp);

        GLint mvp_uniform =  glGetUniformLocation(new_model->shader,"MVP");

        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

        glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

        GLenum error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("draw error\n");
            LOGW("Error %08x \n",error);
        }
    }

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
        GLenum error ;
        Element* element = get_element_from_array(elements,i);
        struct Model *new_model;
        struct Model *LOD0 = element->model;
        new_model = LOD0;
        if(new_model->change_LOD){
            new_model = new_model->LOD;          
        }
        if(LOD0->change_to_HLOD){
            //new_model = LOD0->HLOD;
        }
        
        glUseProgram(new_model->shader);
        
        GLint uniform_color =  glGetUniformLocation(new_model->shader,"color");
        if(element->selected){            
            
            glUniform4fv(uniform_color, 1, red);
        }else
        {
           glUniform4fv(uniform_color, 1, white);
        }
        
        
        glBindTexture(GL_TEXTURE_2D, LOD0->texture.id);
      
        mat4 mvp;
        glm_mat4_identity(mvp);
        
        update_mvp(LOD0->model_mat, mvp);

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

}

void create_models_shaders(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {
        struct Model* new_model = &new_level.models_array.models[i];        

        new_model->shader = glCreateProgram();
        glAttachShader(new_model->shader, standart_vertex_shader);
        glAttachShader(new_model->shader, standart_fragment_shader);
        glLinkProgram(new_model->shader);

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
void load_models_texture_to_gpu(ModelArray* models_array){
    for(size_t i = 0; i < models_array->count ; i++) {
        struct Model *model = &models_array->models[i];


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

    free(new_model->vertex_array.vertices);
    free(new_model->index_array.indices);
    new_model->vertex_array.vertices = NULL;
    new_model->index_array.indices = NULL;
}

void init_models(ModelArray* array){
    for(size_t i = 0; i < array->count ; i++){

        struct Model* new_model = &array->models[i];

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
        free(new_model->index_array.indices);
    }

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

    //sleep(2);


}

void init_game_engine(){
    should_close = false;

    load_level("level01.lvl",&new_level);

    init_camera();

    init_gui();

    glEnable(GL_DEPTH_TEST);

    load_models_texture_to_gpu(&new_level.models_array);
    create_models_shaders();
    init_models(&new_level.models_array);

    init_game();
}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_frame();
    draw_gui();
    update_game();

}

