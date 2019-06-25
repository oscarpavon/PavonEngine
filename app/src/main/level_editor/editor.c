#include "editor.h"

#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <pthread.h>
#include <string.h>
#include "../vector.h"

#include "../engine.h"

typedef struct Element{
    vec3 position;

}Element;

#define COMMAND_ADD_ELEMENT 0
#define COMMAND_REMOVE_ELEMENT 1
#define COMMAND_SAVE_LEVEL 2
#define COMMAND_LOAD_LEVEL 3

ModelArray editor_elements;
bool can_draw;
bool can_load_model;

void add_element(){
    struct Model new_model;
    load_model("../assets/lince.gltf",&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
     standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    add_model_to_array(&editor_elements,new_model);

    init_models(&editor_elements);
    can_draw = true;
    can_load_model = false;
    printf("model loaded and shader created \n");
}

void delete_element(){

}

void save_level(){
    FILE* new_file = fopen("new_level.lvl","w+");
    fputs("Tfirst level", new_file);
    fclose(new_file);

}

void load_level_in_editor(){

}



void handle_command(unsigned short int command){
    switch (command)
    {
    case COMMAND_ADD_ELEMENT:
       can_load_model = true;
        break;
    
    default:
        break;
    }
}

void* input_thread(void* in){
    printf("input thread crated \n");
    char command[100];
    unsigned short int command_code = 50;
    while (1)
    {
         gets(command);
         printf("Input command: %s\n", command);
         if(strcmp(command, "a") == 0){
             command_code = COMMAND_ADD_ELEMENT;
         }else if (strcmp(command, "save") == 0){
             command_code = COMMAND_SAVE_LEVEL;
         }
         handle_command(command_code);
    }
    
   


}

void create_input_thread(){
    pthread_t input_thread_id;
    pthread_create(&input_thread_id,NULL, input_thread, NULL);

}

void check_user_input_command(){

}


void init_editor(){
    init_model_array(&editor_elements, 1);
    can_draw = false;
    can_load_model = false;
}

ModelArray* models_to_draw;
void update_model_to_draw(){
    
}

void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(can_draw)
        draw_models(&editor_elements);
    
    if(can_load_model)
        add_element();
   
}