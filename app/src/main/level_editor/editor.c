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

#include "../images.h"

#define COMMAND_ADD_ELEMENT 0
#define COMMAND_REMOVE_ELEMENT 1
#define COMMAND_SAVE_LEVEL 2
#define COMMAND_LOAD_LEVEL 3

ModelArray editor_models;
ModelArray gizmos;
bool can_draw;


Array editor_elements;

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
    load_models_texture_to_gpu(&gizmos);

    

}

void add_editor_element(const char* path_to_element){
    open_file = false;

    struct Model new_model;
    const char* model_path = path_to_element;
    int result = load_model(model_path,&new_model);
    if(result==-1){
        return;
    }
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
    standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    init_model(&new_model);

    add_model_to_array(&editor_models,new_model);
    
    Element new_element;
    glm_vec3_copy((vec3){0,0,0}, new_element.position);
    new_element.model = &editor_models.models[0];
    new_element.id = element_id_count;
    new_element.model_path = model_path;

    element_id_count++;

    

    add_element_to_array(&editor_elements,&new_element);

    selected_element = (Element*)get_element_from_array(&editor_elements,editor_elements.count-1);

    //init_models(&editor_elements);
   
    can_draw = true;
    
    printf("model loaded and shader created \n");
}

void delete_element(){

}

void add_editor_texture(const char* image_path){
    add_texture = false;
    selected_element = (Element*)get_element_from_array(&editor_elements,editor_elements.count-1);
    selected_element->model->texture.image = load_image(image_path);
    load_model_texture_to_gpu(selected_element->model);
    
}

void add_element_to_save_data(FILE* new_file, Element* selected_element, int index){
    if(index !=0){
        fputs(",\n", new_file);
    }
    fputs("{\n\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",selected_element->id);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",selected_element->position[0] , selected_element->position[1] , selected_element->position[2]);
    fputs("\t\"path\" : ", new_file);
    fprintf(new_file,"\"%s\"\n",selected_element->model_path);
    fputs("}",new_file);
}

void save_data(){
    FILE* new_file = fopen("new_level.lvl","w+");
        
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        //printf("Element name: %s\n", element->model_path);
        add_element_to_save_data(new_file,element,i);
    }


    fclose(new_file);

}

void load_level_in_editor(){
    

    FILE* level_file = fopen("new_level.lvl","r");

    int return_number = 0;

    //fscanf(level_file,"%d", &return_number);


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



void get_elements_in_editor_map(){
    printf("Elements count: %i\n", editor_elements.count);

    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        printf("Element name: %s\n", element->model_path);
    }

}
void get_element_status(Element* element){
    printf("Position: %f, %f %f\n",element->position[0] , element->position[1] , element->position[2]);

}


void init_editor(){
    
    init_vec3(0,3,0, camera_position);
    update_look_at();

    init_model_array(&editor_models, 1);
    init_model_array(&gizmos,1);

    init_array(&editor_elements,sizeof(Element));

    load_editor_element("editor/transform.gltf");

    init_text_renderer();

    can_draw = false;


    element_id_count = 24;    

    editor_mode = DEFAULT_MODE;

    editor_mode_show_text = "Default Mode";

    init_input();

    add_texture = false;
}

void draw_gizmos(){
    draw_models(&gizmos);
}


void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

    if(can_draw)
        draw_models(&editor_models);  
      

    glClear(GL_DEPTH_BUFFER_BIT);
    draw_gizmos();

    text_renderer_loop();   
    
}


void handle_command(unsigned short int command){
    switch (command)
    {
    case COMMAND_ADD_ELEMENT:
       
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