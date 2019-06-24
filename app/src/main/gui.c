//
// Created by pavon on 6/23/19.
//

#include "gui.h"
#include <cglm.h>
#include "camera.h"

#include "input.h"

#include "shader.h"

#include <string.h>
#include <GLES2/gl2.h>

typedef unsigned short int ui_size;

GLuint vert_shader;
GLuint frag_shader;

static GLuint vertex_buffer_id;
VertexArray vertex_array;


void compile_shaders(){
    vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
}

void create_gui_shaders(){
    for(size_t i = 0; i < buttons.count ; i++) {

        size_t offset = buttons.element_bytes_size;

        Button *button = &buttons.data[0] + (i * offset);

        button->shader = glCreateProgram();
        glAttachShader( button->shader, vert_shader);
        glAttachShader( button->shader, frag_shader);
        glLinkProgram( button->shader);
    }


}

void check_if_pressed(struct Button* button){
    ui_size minx = button->position[0] - button->size[0];
    ui_size maxx = button->position[0] + button->size[0];

    ui_size miny = button->position[1] - button->size[1];
    ui_size maxy = button->position[1] + button->size[1];

    if(minx <= touch_position_x && maxy >= touch_position_y){
        if(miny <= touch_position_y && maxx >= touch_position_x){
            button->pressed = true;
            //LOGW("button pressed");
        }
    }else{
        button->pressed = false;
    }
}

void update_button_matrix(Button* button){
    mat4 scale;
    glm_mat4_identity(scale);

    mat4 position;
    glm_mat4_identity(position);

    mat4 projection;
    glm_mat4_identity(projection);

    glm_ortho(0,camera_width_screen,camera_heigth_screen,0,0,1,projection);

    glm_scale(scale,(vec3){button->size[0],button->size[1],0});
    glm_translate(position,(vec3){button->position[0],button->position[1],0});

    mat4 model;
    glm_mul(position, scale, model);

    mat4 model_projection;
    glm_mat4_identity(model_projection);

    glm_mul(projection, model, model_projection);


    GLint mvp_uniform =  glGetUniformLocation(button->shader,"MVP");
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &model_projection[0][0]);


}

void draw_button(){

    for(size_t i = 0; i < buttons.count ; i++){

        size_t offset = buttons.element_bytes_size;

        Button* button = &buttons.data[0] + (i*offset);

        glUseProgram(button->shader);
        glBindTexture(GL_TEXTURE_2D,0);

        update_button_matrix(button);

        glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }


    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("draw error\n");
        LOGW("Error %08x \n",error);
    }


}

void create_vertex_buffer(){
    glGenBuffers(1,&vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof(struct Vertex) , vertex_array.vertices, GL_STATIC_DRAW);
    free(vertex_array.vertices);

}

void init_button(Button* button, float position_x , float position_y , float size_x, float size_y){
    button->position[0] = position_x;
    button->position[1] = position_y;

    button->size[0] = size_x;
    button->size[1] = size_y;
}

void init_gui_element_geometry(){
    struct Vertex vert1;
    init_vec3(-1.0F,1.0,0.0, vert1.postion);
    struct Vertex vert2;
    init_vec3(-1.0F,-1.0F,0.0, vert2.postion);
    struct Vertex vert3;
    init_vec3(1.0,1.0,0.0, vert3.postion);
    struct Vertex vert4;
    init_vec3(1.0,-1.0F,0.0, vert4.postion);


    init_vertex_array(&vertex_array, 1);
    add_vextex_to_array(&vertex_array,vert1);
    add_vextex_to_array(&vertex_array,vert2);
    add_vextex_to_array(&vertex_array,vert3);
    add_vextex_to_array(&vertex_array,vert4);
}
void init_gui(){

    init_gui_element_geometry();

    compile_shaders();

    create_vertex_buffer();

    init_button(&button1, 600, 100, 35,35);

    init_button(&back_button, 600, 300, 35,35);


    init_array(&buttons, sizeof(Button));
    add_element_to_array(&buttons,&button1);
    add_element_to_array(&buttons,&shoot_button);
    add_element_to_array(&buttons,&back_button);

    create_gui_shaders();

}

void draw_gui(){
    draw_button();
    check_if_pressed(&button1);
    check_if_pressed(&back_button);
}