#include "editor.h"

#include <stdlib.h>
#include <stdio.h>
#include "../model.h"
#include "../shader.h"

#include <string.h>
#include "../vector.h"

#include "../engine.h"

#include "text.h"

#include "../camera.h"

#include "../images.h"

#include "skeletal_editor.h"


ModelArray editor_models;

bool can_draw;

ModelArray LOD_models;

CameraElement camera_in_editor;

Model* selected_model;

void deselect_all(){
    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = get_element_from_array(&editor_elements,i);
        element->selected = false;
    }
}

void select_last_element(){
    if(selected_element != NULL)
        selected_element->selected = false;
    selected_element = (Element*)get_element_from_array(&editor_elements,editor_elements.count-1);
    selected_element->selected = true;
}

void editor_message(const char* message){
    set_text_size(12);
    render_text(message , 0 + (-(camera_width_screen/2)) * pixel_size_x , 0 + (-(camera_heigth_screen/2)+12) * pixel_size_y  , pixel_size_x, pixel_size_y, false);   
}

void add_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));

    new_element.duplicated_of_id = -1;
    new_element.model = &editor_models.models[element_id_count];
    new_element.id = element_id_count;
    new_element.type = ELEMENT_TYPE_MODEL;

    glm_vec3_copy((vec3){0,0,0}, new_element.position);
    
    glm_quat_identity(new_element.rotation);    

    element_id_count++;
    
    add_element_to_array(&editor_elements,&new_element);

    select_last_element();
}

void add_empty_model(){
    Model new_model;
    memset(&new_model,0,sizeof(Model));
    add_model_to_array(&editor_models,new_model);
    selected_model = &editor_models.models[editor_models.count-1];
    glm_mat4_identity(selected_model->model_mat);
}

void add_editor_native_element(const char* native_element_name){
    if( strcmp("Camera", native_element_name) == 0 ){
        add_empty_model();
        add_element();
        selected_model->draw = false;
        strcpy(selected_element->name, "Camera01");
        selected_element->type = ELEMENT_TYPE_CAMERA;
        can_draw = true;
    }
}

void add_editor_element(const char* model_gltf_path){
    if(model_gltf_path == NULL){
        printf("Error to load, null path (add_editor_element - 50\n");
        return;
    }
    struct Model new_model[3];
    memset(new_model,0,sizeof(new_model));

    const char* model_path = model_gltf_path;
    int result = load_model(model_path,new_model);
    if(result==-1){
        return;
    }    
    

    struct Model* model0 = &new_model[0];
  
    glm_mat4_identity(model0->model_mat);   

    model0->shader = glCreateProgram();

    glAttachShader(model0->shader, standart_vertex_shader);
    glAttachShader(model0->shader, editor_standard_fragment_shader);
    glLinkProgram(model0->shader);

    init_model(model0);
    model0->actual_LOD = 0;
    model0->change_LOD = false;
    model0->draw = true;
    

    if(new_model[0].LOD_count >= 1){
        for(int i = 0; i < new_model[0].LOD_count; i++){
            glm_mat4_identity(new_model[i+1].model_mat);  
            init_model(&new_model[i+1]);
            new_model[i+1].shader = model0->shader;
            add_model_to_array(&LOD_models,new_model[i+1]);
        }
    }
    if(new_model[0].has_HLOD){
        glm_mat4_identity(new_model[2].model_mat);  
        init_model(&new_model[2]);
        new_model[2].shader = model0->shader;
        add_model_to_array(&LOD_models,new_model[2]);
        new_model->HLOD = &LOD_models.models[LOD_models.count-1];
    }
    
    add_model_to_array(&editor_models,new_model[0]);

    add_element();

    strcpy(selected_element->model_path,model_path);
    
    if(model0->LOD_count >= 1){
        selected_element->has_LOD  = true;
    }
    
    can_draw = true;
    
    printf("model loaded and shader created \n");
}


void clean_element(Element* element){    
    if(element->model->skeletal != NULL){
        free(element->model->skeletal->joints);
        free(element->model->skeletal);        
    }
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

    if(selected_element == NULL){
        printf("No element selected\n"); 
        return;
    }
    if(image_path == NULL){
        printf("Error to load, null path (add_editor_texture - 154\n");
        return;
    }

    selected_element->model->texture.image = load_image(image_path);
    load_model_texture_to_gpu(selected_element->model);
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


void set_selected_element_transform(vec3 position, versor rotation){
    glm_translate(selected_element->model->model_mat, position);
    glm_vec3_add(selected_element->position,position,selected_element->position);

    mat4 model_rot_mat;
    glm_quat_mat4(rotation,model_rot_mat);

    glm_mul(selected_element->model->model_mat, model_rot_mat, selected_element->model->model_mat);

    glm_quat_copy(rotation, selected_element->rotation);
}

void update_camera_aspect_ratio(){
    glm_perspective(45.f, camera_width_screen / camera_heigth_screen , 0.001f , 5000.f , main_camera.projection);
}

Array load_elements;
#define CAMERA__ELEMENT_ID 300
void add_loaded_elements_to_editor(){
    init_model_array(&editor_models,load_elements.count);
    for(int i = 0; i < load_elements.count; i++){
        Element* element = &load_elements.data[i*load_elements.element_bytes_size];
        if(element->duplicated_of_id==-1 && element->id != CAMERA__ELEMENT_ID){
            add_editor_element(element->model_path);        
            add_editor_texture(element->texture_path);
        }else if(element->id != CAMERA__ELEMENT_ID){            //Duplicated element
            Model new_model;
            memset(&new_model,0,sizeof(struct Model));
            add_model_to_array(&editor_models,new_model);

            struct Model* from = &editor_models.models[element->duplicated_of_id];
            struct Model* model_copy = &editor_models.models[editor_models.count-1];   
            memmove(model_copy,from,sizeof(struct Model));                    

            element_id_count++;
            Element new_element;
            memcpy(&new_element, element, sizeof(struct Element));
            if(element->duplicated_of_id > -1)
                new_element.model = &editor_models.models[editor_models.count-1];            
            new_element.id = element_id_count;
            new_element.duplicated_of_id = element->duplicated_of_id;

            add_element_to_array(&editor_elements,&new_element);
            selected_element = get_element_from_array(&editor_elements,editor_elements.count-1);
        }
        if(element->id != CAMERA__ELEMENT_ID && element->duplicated_of_id == -1){
            set_selected_element_transform(element->position,element->rotation);
        }else if(element->id != CAMERA__ELEMENT_ID && element->duplicated_of_id > -1){
            glm_translate(selected_element->model->model_mat, element->position);
        }
        if(element->id == CAMERA__ELEMENT_ID){
            glm_vec3_copy(element->position,camera_position);
            update_look_at();
        }
    }
}

void load_level_in_editor(const char* name){    

    FILE* level_file = fopen(name,"r");

    if(level_file == NULL){
        printf("Level not found: %s\n",name);
        return;
    }
    int return_number = 0;

    fseek(level_file, 0, SEEK_END);
    long file_size = ftell(level_file);
    rewind (level_file);

    char file_buffer[file_size];

    fread(file_buffer,sizeof(char), file_size, level_file);

    fclose(level_file);

    load_level_elements_from_json(file_buffer,file_size, &load_elements);

    add_loaded_elements_to_editor();
    
    clean_array(&load_elements);
    
}

void duplicate_selected_element(){
    Element new_element;
    memset(&new_element,0,sizeof(struct Element));
    

    Model new_model;
    memset(&new_element,0,sizeof(struct Model));

    memcpy(&new_model,selected_element->model,sizeof(struct Model));
    new_model.has_HLOD = false;
    new_model.HLOD = NULL;
    new_model.change_to_HLOD = false;
    add_model_to_array(&editor_models,new_model);
    
    element_id_count++;
    
    memcpy(&new_element, selected_element, sizeof(struct Element));

    new_element.model = &editor_models.models[editor_models.count-1];
    new_element.id = element_id_count;
    new_element.has_HLOD = false;

    if(selected_element->duplicated_of_id > -1)
        new_element.duplicated_of_id = selected_element->duplicated_of_id;
    else
        new_element.duplicated_of_id = selected_element->id;

    add_element_to_array(&editor_elements,&new_element);
    
    select_last_element();
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

void remove_selected_element(){
    remove_last_element_from_model_array(&editor_models);
    clean_element(selected_element);
    selected_element = NULL;
    remove_element_from_array(&editor_elements);
    element_id_count--;
}

void reload_editor(){
    element_id_count = 0;
    clean_model_array(&editor_models);
    clean_array(&editor_elements);   
    
}

void init_editor(){
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    init_vec3(0,3,0, camera_position);
    update_look_at();

    init_model_array(&editor_models, 1);
 

    init_array(&editor_elements,sizeof(Element));

    init_gizmos();


    editor_standard_fragment_shader = compile_shader(editor_standard_fragment_shader_source, GL_FRAGMENT_SHADER);

    init_text_renderer();

    can_draw = false;
    
    init_array(&selected_elements_id,sizeof(unsigned short int));

    element_id_count = 0;    

    editor_mode = EDITOR_DEFAULT_MODE;

    editor_mode_show_text = "Default Mode";

    init_input();

    camera_velocity = 0.04;

    init_model_array(&LOD_models,1);

    init_skeletal_editor();
    
}


void check_elements_camera_distance_for_LOD(){
    for(int i = 0; i< editor_elements.count ; i++){
        Element* element = get_element_from_array(&editor_elements,i);
        if(element->has_LOD){
            float camera_distance = glm_vec3_distance(element->position, camera_position);
            //printf("Camera Distance: %f\n",camera_distance);
            if(camera_distance >= 8){
                element->model->change_LOD  = true;
            }else if (camera_distance <= 7.6){
                element->model->change_LOD  = false;
            }
            if(camera_distance >= 20 && element->model->has_HLOD){
                element->model->change_to_HLOD = true;
            }
            if(camera_distance < 19){
                element->model->change_to_HLOD = false;
            }
        }        
    }
}

void assign_LOD_mesh(){
    for(int i = 0; i < editor_models.count ; i++){
        struct Model* model = &editor_models.models[i];
        if(model->change_LOD){
            model->LOD = &LOD_models.models[model->actual_LOD];
        }
    }
}


void update_editor(){
    glClearColor(1,0.5,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    check_elements_camera_distance_for_LOD();
    assign_LOD_mesh();
    if(can_draw)
        draw_elements(&editor_elements);  
      

    glClear(GL_DEPTH_BUFFER_BIT);

    draw_gizmos();

    text_renderer_loop();

    editor_message("editor message");

    

   
}
