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
#include "images.h"

#include "engine.h"


typedef unsigned short int ui_size;

GLuint vert_shader;
GLuint frag_shader;

GLuint gui_vertex_buffer_id = -1;
Array gui_vertex_array;

GLuint logo_texture_id;

Array buttons;

void compile_shaders(){
    vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
}

void create_gui_shaders(){
    for(size_t i = 0; i < buttons.count ; i++) {

        size_t offset = buttons.element_bytes_size;

        Button *button = &buttons.data[0] + (i * offset);

        button->shader = create_engine_shader(vert_shader,frag_shader);
      
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
            LOGW("button pressed\n");
            
        }
    }else{
        button->pressed = false;
    }
}

void update_button_matrix(GLuint shader_id, vec2 size, vec2 position){
    mat4 scale;
    glm_mat4_identity(scale);

    mat4 position_mat;
    glm_mat4_identity(position_mat);

    mat4 projection;
    glm_mat4_identity(projection);

    glm_ortho(0,camera_width_screen,camera_heigth_screen,0,0,1,projection);

    glm_scale(scale,(vec3){size[0],size[1],0});

    glm_translate(position_mat,(vec3){position[0],position[1],0});


    mat4 model;
    glm_mul(position_mat, scale, model);

    mat4 model_projection;
    glm_mat4_identity(model_projection);

    glm_mul(projection, model, model_projection);


    GLint mvp_uniform =  glGetUniformLocation(shader_id,"MVP");
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &model_projection[0][0]);


}

void draw_buttons(){

    for(size_t i = 0; i < buttons.count ; i++){

        size_t offset = buttons.element_bytes_size;

        Button* button = &buttons.data[0] + (i*offset);

        glUseProgram(button->shader);
        glBindTexture(GL_TEXTURE_2D,0);

        update_button_matrix(button->shader, button->size, button->position);

        glBindBuffer(GL_ARRAY_BUFFER,gui_vertex_buffer_id);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        
    }


    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("buttons error\n");
        LOGW("Error %08x \n",error);
    }


}

void create_vertex_buffer(){
    glGenBuffers(1,&gui_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,gui_vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, gui_vertex_array.count * sizeof(struct Vertex) , gui_vertex_array.data, GL_STATIC_DRAW);
    free_to_marker(previous_marker);
    //free(gui_vertex_array.vertices);
    
    //gui_vertex_array.vertices = NULL;

}

void init_button(Button* button, float position_x , float position_y , float size_x, float size_y){
    char name[] = "NewButton";
    memset(button->name, 0, strlen(button->name));
    memcpy(button->name,name,strlen(name));
    
    button->position[0] = position_x;

    button->size[0] = size_x;
    button->size[1] = size_y;

    button->original_position[0] = position_x;
    if(button->relative_to == POSITION_RELATIVE_LEFT_BOTTON){
        button->original_position[1] = camera_heigth_screen - position_y;
        button->position[1] = camera_heigth_screen - position_y;
    }
    else{
        button->original_position[1] = position_y;
        button->position[1] = position_y;
    }
}

void init_gui_element_geometry(){
    struct Vertex vert1;
    init_vec3(-1.0F,1.0,0.0, vert1.postion);
    vert1.uv[0] = 0;
    vert1.uv[1] = 1;

    struct Vertex vert2;
    init_vec3(-1.0F,-1.0F,0.0, vert2.postion);
    vert2.uv[0] = 0;
    vert2.uv[1] = 0;

    struct Vertex vert3;
    init_vec3(1.0,1.0,0.0, vert3.postion);
    vert3.uv[0] = 1;
    vert3.uv[1] = 1;

    struct Vertex vert4;
    init_vec3(1.0,-1.0F,0.0, vert4.postion);
    vert4.uv[0] = 1;
    vert4.uv[1] = 0;


    init_array(&gui_vertex_array, sizeof(Vertex),4);
    add_to_array(&gui_vertex_array,&vert1);
    add_to_array(&gui_vertex_array,&vert2);
    add_to_array(&gui_vertex_array,&vert3);
    add_to_array(&gui_vertex_array,&vert4);
}
void init_gui(){

    init_array(&buttons, sizeof(Button),20);

    create_gui_shaders();

    actual_buttons_array = &buttons;
}

GLuint logo_shader;

void create_logo_shader(){
    logo_shader = create_engine_shader(vert_shader,frag_shader);

}

void draw_logo_image(){
    glEnable(GL_BLEND);  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Button load_screen_button;

    load_screen_button.relative_to = 400;
    init_button(&load_screen_button,camera_width_screen/2 , camera_heigth_screen / 2, 250, 250);

    Button* button = &load_screen_button;

    button->shader = logo_shader;

    glUseProgram(button->shader);
    glBindTexture(GL_TEXTURE_2D,logo_texture_id);

    update_button_matrix(button->shader, button->size, button->position);

    glBindBuffer(GL_ARRAY_BUFFER,gui_vertex_buffer_id);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("draw logo\n");
        LOGW("Error %08x \n",error);
    }

    //glDisable(GL_BLEND);
}


void draw_logo(){

    Image logo_image;
    int result =  load_image_with_format("white_logo.png",GL_RGBA, &logo_image);
    if(result == -1)
        return;
        
    
    glGenTextures(1, &logo_texture_id);
    glBindTexture(GL_TEXTURE_2D, logo_texture_id);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, logo_image.width,
                  logo_image.heigth, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, logo_image.pixels_data);

    free_image(&logo_image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    init_gui_element_geometry();

    compile_shaders();

    create_vertex_buffer();

    create_logo_shader();

    draw_logo_image();
}

void draw_loading_screen(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_logo();
}

void update_user_iterface_status(){
     for(size_t i = 0; i < buttons.count ; i++){
         Button* button = get_from_array(&buttons,i);
         check_if_pressed(button);
         if(button->pressed){
             ActionPointer* action_pointer = get_from_array(&actions_pointers,button->action_function_id);
             if(action_pointer->action == NULL){
                 printf("No button action added\n");
                 return;
             }
             action_pointer->action();
         }
     }
}

void draw_gui(){
    glCullFace(GL_FRONT);
    draw_buttons();

    //draw_logo_image();
    
    //check_if_pressed(&button1);   

    glCullFace(GL_BACK);
}

void new_empty_button(){
    Button new_button;

    init_button(&new_button, 400, 300, 35,35);

    add_to_array(actual_buttons_array,&new_button);

    Button* button = get_from_array(actual_buttons_array,actual_buttons_array->count-1);
    button->shader = create_engine_shader(vert_shader,frag_shader);
    button->action_function_id = 0;
    button->pressed = false;
}

void load_gui(const char* name){
    char save_name[50];
    memset(save_name,0,sizeof(save_name));
    strcat(save_name, gui_folder);
    strcat(save_name,name);
    strcat(save_name,".gui");

    File level_file;
    if( load_file(save_name, &level_file) == -1){
        
        printf("GUI file not found: %s\n",name);
        return;
    }

    parse_gui_file(level_file.data , level_file.size_in_bytes , &buttons);

    close_file(&level_file);

    for(int i = 0; i < buttons.count; i++){
        Button* button = get_from_array(&buttons,i);
        button->shader = create_engine_shader(vert_shader,frag_shader);
        button->pressed = false;
    }
}