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
#include "camera.h"



static inline void update_mvp(mat4 model, mat4 mvp_out){
    mat4 projection_view;
    glm_mul(main_camera.projection , main_camera.view, projection_view);
    glm_mul(projection_view , model , mvp_out);

}

void draw_frame(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {

        struct Model *new_model = &new_level.models_array.models[i];

        glUseProgram(new_model->shader);
        glBindTexture(GL_TEXTURE_2D, new_model->texture.id);

        mat4 mvp;
        glm_mat4_identity(mvp);

        //glm_rotate(&new_level.models_array.models[2].model_mat, 0.005f, (vec3){0,0,1});
        update_mvp(new_model->model_mat, mvp);

        GLint mvp_uniform =  glGetUniformLocation(new_model->shader,"MVP");
        //glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &new_model->model_mat[0][0]);
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

void create_models_shaders(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {
        struct Model* new_model = &new_level.models_array.models[i];

        GLuint vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
        GLuint frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);

        new_model->shader = glCreateProgram();
        glAttachShader(new_model->shader, vert_shader);
        glAttachShader(new_model->shader, frag_shader);
        glLinkProgram(new_model->shader);

    }

}

void load_model_texture_to_gpu(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {
        struct Model *model = &new_level.models_array.models[i];


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
void init_level_models(){
    for(size_t i = 0; i < new_level.models_array.count ; i++){

        struct Model* new_model = &new_level.models_array.models[i];

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

void init_engine(){
    should_close = false;

    load_level("level01.lvl",&new_level);

    init_camera();

    init_gui();

    glEnable(GL_DEPTH_TEST);


    load_model_texture_to_gpu();
    create_models_shaders();
    init_level_models();

    init_game();

}

void engine_loop(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //glm_rotate(mvp, 0.005f, axis);

    draw_frame();
    draw_gui();
    update_game();

}

void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //glm_rotate(mvp, 0.005f, axis);

    draw_frame();
   
}