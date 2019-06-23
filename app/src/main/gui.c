//
// Created by pavon on 6/23/19.
//

#include "gui.h"
#include <cglm.h>
#include "camera.h"

void update_button_matrix(){
    mat4 scale;
    glm_mat4_identity(scale);

    mat4 position;
    glm_mat4_identity(position);

    mat4 projection;
    glm_mat4_identity(projection);

    glm_ortho(0,camera_width_screen,camera_heigth_screen,0,0,1,projection);

    glm_scale(scale,(vec3){35,35,0});
    glm_translate(position,(vec3){1000,250,0});

    mat4 model;
    glm_mul(position, scale, model);

    mat4 model_projection;
    glm_mat4_identity(model_projection);

    glm_mul(projection, model, model_projection);


    GLint mvp_uniform =  glGetUniformLocation(button1.shader,"MVP");
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &model_projection[0][0]);


}

void draw_button(){
    glUseProgram(button1.shader);
    glBindTexture(GL_TEXTURE_2D,0);


    update_button_matrix();


    glBindBuffer(GL_ARRAY_BUFFER,button1.vertex_buffer);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("draw error\n");
        LOGW("Error %08x \n",error);
    }
}

void create_vertex_buffer(){
    glGenBuffers(1,&button1.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER,button1.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, button1.vertex_array.count * sizeof(struct Vertex) , button1.vertex_array.vertices, GL_STATIC_DRAW);


}

void init_button(){
    struct Vertex vert1;
    init_vec3(-1.0F,1.0,0.0, vert1.postion);
    struct Vertex vert2;
    init_vec3(-1.0F,-1.0F,0.0, vert2.postion);
    struct Vertex vert3;
    init_vec3(1.0,1.0,0.0, vert3.postion);
    struct Vertex vert4;
    init_vec3(1.0,-1.0F,0.0, vert4.postion);


    init_vertex_array(&button1.vertex_array, 1);
    add_vextex_to_array(&button1.vertex_array,vert1);
    add_vextex_to_array(&button1.vertex_array,vert2);
    add_vextex_to_array(&button1.vertex_array,vert3);
    add_vextex_to_array(&button1.vertex_array,vert4);


    create_vertex_buffer();


}
