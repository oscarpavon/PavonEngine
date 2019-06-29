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

#include "skeletal_editor.h"

#define COMMAND_ADD_ELEMENT 0
#define COMMAND_REMOVE_ELEMENT 1
#define COMMAND_SAVE_LEVEL 2
#define COMMAND_LOAD_LEVEL 3

ModelArray editor_models;
ModelArray gizmos;
bool can_draw;



Array editor_elements;

unsigned int element_id_count;

void load_editor_element(const char* path_model, const char* color_texture_path){
   

    struct Model new_model;
    load_model(path_model,&new_model);
    glm_mat4_identity(new_model.model_mat);   

    new_model.shader = glCreateProgram();
     standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    glAttachShader(new_model.shader, standart_vertex_shader);
    glAttachShader(new_model.shader, standart_fragment_shader);
    glLinkProgram(new_model.shader);

    new_model.texture.image = load_image(color_texture_path);

    init_model(&new_model);
    //load_models_texture_to_gpu(&gizmos);
    load_model_texture_to_gpu(&new_model);

    add_model_to_array(&gizmos,new_model);
    

    //init_models(&gizmos);  
    

    

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
    new_element.model = &editor_models.models[element_id_count];
    new_element.id = element_id_count;
    glm_quat_identity(new_element.rotation);

    new_element.model_path = malloc(strlen(model_path));
    strcpy(new_element.model_path,model_path);

    element_id_count++;
    

    add_element_to_array(&editor_elements,&new_element);

    selected_element = (Element*)get_element_from_array(&editor_elements,editor_elements.count-1);    
   
    can_draw = true;
    
    printf("model loaded and shader created \n");
}


void clean_element(Element* element){
    free(element->texture_path);
    free(element->model_path);
}

void clean_editor(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        clean_element(element);
        
    }
    free(editor_elements.data);

    clean_skeletal_editor();
}

void add_editor_texture(const char* image_path){
    add_texture = false;
    //selected_element = (Element*)get_element_from_array(&editor_elements,editor_elements.count-1);
    selected_element->model->texture.image = load_image(image_path);
    load_model_texture_to_gpu(selected_element->model);
    selected_element->texture_path = malloc(strlen(image_path));
    strcpy(selected_element->texture_path,image_path);

    
}

void rotate_editor_element(Element* element, float angle, vec3 axis){
   
    versor new_rot_quat;
    glm_quatv(new_rot_quat, glm_rad(angle), axis);

    versor result_quat;
    glm_quat_mul(element->rotation, new_rot_quat, result_quat);

    glm_quat_copy(result_quat, element->rotation);

    mat4 model_rot_mat;
    glm_quat_mat4(new_rot_quat,model_rot_mat);

    glm_mul(element->model->model_mat, model_rot_mat, element->model->model_mat);

}

void add_element_to_save_data(FILE* new_file, Element* selected_element, int index){
    if(index !=0){
        fputs(",\n", new_file);
    }
    fputs("{\n\t\"id\" : ", new_file);
    fprintf(new_file,"%i,\n",selected_element->id);
    fputs("\t\"pos\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f],\n",selected_element->position[0] , selected_element->position[1] , selected_element->position[2]);
    fputs("\t\"rot\" : ", new_file);
    fprintf(new_file,"[%f , %f , %f , %f],\n",selected_element->rotation[0] , selected_element->rotation[1] , selected_element->rotation[2], selected_element->rotation[3]);
    fputs("\t\"path\" : ", new_file);
    fprintf(new_file,"\"%s\",\n",selected_element->model_path);
    fputs("\t\"texture\" : ", new_file);
    fprintf(new_file,"\"%s\"\n",selected_element->texture_path);
    fputs("}",new_file);
}
void save_level_info(FILE* new_file){
    fputs("{\n\t\"model_count\" : ", new_file);
    fprintf(new_file,"%i",element_id_count);
    fputs("\n},\n",new_file);
}

void create_save_data_backup(){
    FILE* level_file = fopen("new_level.lvl","r");
    fseek(level_file, 0, SEEK_END);
    long file_size = ftell(level_file);
    rewind (level_file);

    char file_buffer[file_size];

    fread(file_buffer,sizeof(char), file_size, level_file);

    FILE* new_file = fopen("level.bkp1","w");
    fputs(file_buffer,new_file);
    fclose(new_file);

    fclose(level_file);
}
void save_data(){
    create_save_data_backup();
    FILE* new_file = fopen("new_level.lvl","w+");
    save_level_info(new_file);   
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        //printf("Element name: %s\n", element->model_path);
        add_element_to_save_data(new_file,element,i);
    }
    fclose(new_file);

}

void set_selected_element_transform(vec3 position, versor rotation){
    glm_translate(selected_element->model->model_mat, position);
    glm_vec3_add(selected_element->position,position,selected_element->position);

    mat4 model_rot_mat;
    glm_quat_mat4(rotation,model_rot_mat);

    glm_mul(selected_element->model->model_mat, model_rot_mat, selected_element->model->model_mat);

    glm_quat_copy(rotation, selected_element->rotation);
}

Array load_elements;

void add_loaded_elements_to_editor(){
    for(int i = 0; i < load_elements.count; i++){
        Element* element = &load_elements.data[i*load_elements.element_bytes_size];
        add_editor_element(element->model_path);
        add_editor_texture(element->texture_path);
        set_selected_element_transform(element->position,element->rotation);
    }
}

void load_level_in_editor(){    

    FILE* level_file = fopen("new_level.lvl","r");

    int return_number = 0;

    fseek(level_file, 0, SEEK_END);
    long file_size = ftell(level_file);
    rewind (level_file);

    char file_buffer[file_size];

    fread(file_buffer,sizeof(char), file_size, level_file);

    fclose(level_file);

    load_level_elements_from_json(file_buffer,file_size, &load_elements);

    add_loaded_elements_to_editor();
    
    free(load_elements.data);
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

void select_editor_elemenent(int id){
    printf("Slected edito0r element \n");
    Element* element = NULL;
    if(id == 0){
        element = (Element*)&editor_elements.data[0];
        element->model = &editor_models.models[0];
    }        
    element = (Element*)get_element_from_array(&editor_elements,id);
    element->model = &editor_models.models[id];
    selected_element = element;
}

void remove_selected_element(){
    remove_last_element_from_model_array(&editor_models);
    clean_element(selected_element);
    selected_element = NULL;
    remove_element_from_array(&editor_elements);
}

void init_editor(){
    
    init_vec3(0,3,0, camera_position);
    update_look_at();

    init_model_array(&editor_models, 1);
    init_model_array(&gizmos,1);

    init_array(&editor_elements,sizeof(Element));

    load_editor_element("editor/transform.gltf","editor/transform_gizmo.jpg");
    load_editor_element("editor/rotate.gltf", "editor/rotate_gizmo.png");

    init_text_renderer();

    can_draw = false;


    element_id_count = 0;    

    editor_mode = EDITOR_DEFAULT_MODE;

    editor_mode_show_text = "Default Mode";

    init_input();

    add_texture = false;

    editor_element_list_menu.show = false;
    editor_element_list_menu.actual_element_select = 0;
    editor_element_list_menu.execute_function = &select_editor_elemenent;

    can_draw_gizmos = true;
    can_draw_skeletal_bones = true;

    draw_translate_gizmo = false;
    draw_rotate_gizmo = false;
    
}

void draw_gizmos(){
    if(can_draw_gizmos){
        if(draw_translate_gizmo){
            Model* actual_gizmo = &gizmos.models[0];
            if(selected_element != NULL){
                glm_mat4_copy(selected_element->model->model_mat, actual_gizmo->model_mat);
            }
            draw_simgle_model(actual_gizmo);
        }
        if(draw_rotate_gizmo){
            Model* actual_gizmo = &gizmos.models[1];
            if(selected_element != NULL){
                glm_mat4_copy(selected_element->model->model_mat, actual_gizmo->model_mat);
            }
            draw_simgle_model(actual_gizmo);
        }
        
    }
    if(can_draw_skeletal_bones)   
        draw_skeletal_bones();
}



void draw_editor_elements_text_list(){
    float text_size = 12;
    set_text_size(text_size);

     for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_element_from_array(&editor_elements,i);
        //printf("Element name: %s\n", element->model_path);
        int y_pos = i*text_size+text_size;
        if(i == 0){
            y_pos = text_size;
        }
        bool can_mark = false;
        if(editor_element_list_menu.actual_element_select == i)
            can_mark = true;

        render_text(element->model_path,  0 + ((camera_width_screen/2)-100) * pixel_size_x,   1 - (y_pos+100) * pixel_size_y, pixel_size_x, pixel_size_y, can_mark);
    }
       
}

void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

    if(can_draw)
        draw_models(&editor_models);  
      

    glClear(GL_DEPTH_BUFFER_BIT);

    draw_gizmos();

    text_renderer_loop();

    if(editor_element_list_menu.show)   
        draw_editor_elements_text_list();
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