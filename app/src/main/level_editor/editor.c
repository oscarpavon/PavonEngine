#include "editor.h"

#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <pthread.h>
#include <string.h>

typedef struct Element{
    vec3 position;

}Element;

#define COMMAND_ADD_ELEMENT 0
#define COMMAND_REMOVE_ELEMENT 1
#define COMMAND_SAVE_LEVEL 2
#define COMMAND_LOAD_LEVEL 3

void add_element(){
    struct Model new_model;
    load_model("../assets/lince.gltf",&new_model);
    glm_mat4_identity(new_model.model_mat);
    GLuint vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    GLuint frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);

    new_model.shader = glCreateProgram();
    glAttachShader(new_model.shader, vert_shader);
    glAttachShader(new_model.shader, frag_shader);
    glLinkProgram(new_model.shader);

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
       add_element();
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
         if(strcmp(command, "add") == 0){
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

