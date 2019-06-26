#include "editor.h"


#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <pthread.h>
#include <string.h>
#include "../vector.h"

#include "../engine.h"

#include "../third_party/cgltf.h"

#include "text.h"

#include "../camera.h"

#define COMMAND_ADD_ELEMENT 0
#define COMMAND_REMOVE_ELEMENT 1
#define COMMAND_SAVE_LEVEL 2
#define COMMAND_LOAD_LEVEL 3

ModelArray editor_elements;
ModelArray gizmos;
bool can_draw;
bool can_load_model;

Array editor_models;

unsigned int element_id_count;

void load_editor_element(const char* path_model){
   

    struct Model new_model;
    load_model(path_model,&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
     standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    new_model.texture.image = load_image("editor/transform_gizmo.jpg");


    add_model_to_array(&gizmos,new_model);
    

    init_models(&gizmos);
    load_model_texture_to_gpu(&gizmos);

}

void add_element(){
    struct Model new_model;
    const char* model_path = "../assets/lince.gltf";
    load_model(model_path,&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
    standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    add_model_to_array(&editor_elements,new_model);
    
    Element new_element;
    glm_vec3_copy((vec3){0,0,0}, new_element.position);
    new_element.model = &editor_elements.models[0];
    new_element.id = element_id_count;
    new_element.model_path = model_path;

    element_id_count++;

    add_element_to_array(&editor_models,&new_element);

    selected_element = (Element*)get_element_from_array(&editor_models,editor_elements.count-1);

    init_models(&editor_elements);
    can_draw = true;
    can_load_model = false;
    printf("model loaded and shader created \n");
}

void delete_element(){

}

void save_data(){
    FILE* new_file = fopen("new_level.lvl","w+");
    fputs("{\n\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",selected_element->id);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"%f %f %f,\n",selected_element->position[0] , selected_element->position[1] , selected_element->position[2]);
    fputs("\t\"path\" : ", new_file);
    fprintf(new_file,"\"%s\"\n",selected_element->model_path);
    fputs("}",new_file);

    fclose(new_file);

}

void load_level_in_editor(){
    

    FILE* level_file = fopen("new_level.lvl","r");

    fseek(level_file, 0, SEEK_END);
    long file_size = ftell(level_file);
    rewind (level_file);

    char file_buffer[file_size];

    fread(file_buffer,sizeof(char), file_size, level_file);
     
    
    parse_json(file_buffer,file_size);

    Element new_element;
    glm_vec3_copy((vec3){0,0,0}, new_element.position);    
    

    element_id_count++;

    add_element_to_array(&editor_models,&new_element);
    fclose(level_file);
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

void get_element_status(Element* element){
    printf("Position: %f, %f %f\n",element->position[0] , element->position[1] , element->position[2]);

}

void create_input_thread(){
    pthread_t input_thread_id;
    pthread_create(&input_thread_id,NULL, input_thread, NULL);

}

void check_user_input_command(){

}



void init_editor(){

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_model_array(&editor_elements, 1);
    init_model_array(&gizmos,1);

    init_array(&editor_models,sizeof(Element));

    load_editor_element("editor/transform.gltf");

    init_text_renderer();

    can_draw = false;
    can_load_model = false;

    element_id_count = 24;
}

ModelArray* models_to_draw;
void update_model_to_draw(){
    
}

void draw_gizmos(){
    draw_models(&gizmos);
}

void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(can_draw)
        draw_models(&editor_elements);
    
    if(can_load_model)
        add_element();

    float sx = 2.0 / camera_width_screen;
    float sy = 2.0 / camera_heigth_screen;

    render_text("Pavon Studios",  -1 + 8 * sx,   1 - 50 * sy,    sx, sy);
             

    glClear(GL_DEPTH_BUFFER_BIT);
    draw_gizmos();
}