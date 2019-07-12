#include "level.h"

#include "../model.h"
#include "../camera.h"
#include "../engine.h"
#ifdef EDITOR
#include "../level_editor/editor.h"
#endif // EDITOR

#include "../file_loader.h"

#define CAMERA__ELEMENT_ID 300

void add_loaded_elements(Array* load_elements, ModelArray* editor_models, Array* editor_elements){
    init_model_array(editor_models,load_elements->count);
    for(int i = 0; i < load_elements->count; i++){
        Element* element = &load_elements->data[i*load_elements->element_bytes_size];
        switch (element->type)
        {
        case ELEMENT_TYPE_PLAYER_START:{
            new_empty_model();
            new_empty_element();
            selected_element->type = element->type;
            selected_model->draw = false;
            strcpy(selected_element->name, "PlayerStart01");            
            glm_vec3_copy(element->position,selected_element->position);
            glm_vec4_copy(element->rotation,selected_element->rotation);
            glm_translate(selected_model->model_mat,selected_element->position);
            player_start = selected_element;
            continue;
            break;
        }            
            
        case ELEMENT_TYPE_PLAYER_CONTROLLER:{
            new_empty_model();
            new_empty_element();
            selected_model->draw = false;
            selected_element->type = element->type;
            strcpy(selected_element->name, "Player");
            selected_element->model = &actual_model_array->models[element->model_id];           
            selected_element->model_id = element->model_id;
            player1 = selected_element;
            continue;
            break;
        }
        
        default:
            break;
        }        

        if(element->duplicated_of_id==-1 && element->id != CAMERA__ELEMENT_ID){
            if( strcmp(element->model_path, "") == 0){
                printf("[X]No model path in load element\n");
                return;
            }
            if( strcmp(element->texture_path, "") == 0){
                printf("[X]No texture path in load element\n");
                return;
            }
                
            add_element_with_model_path(element->model_path);        
            add_texture_to_selected_element_with_image_path(element->texture_path);

        }else if(element->id != CAMERA__ELEMENT_ID){            //Duplicated elements
            new_empty_model();
            struct Model* from = &editor_models->models[element->duplicated_of_id];             
            memmove(selected_model,from,sizeof(struct Model));              
            new_empty_element();
            glm_vec3_copy(element->position,selected_element->position);
            glm_vec4_copy(element->rotation,selected_element->rotation);
            selected_element->duplicated_of_id = element->duplicated_of_id;        

            if(element->duplicated_of_id > -1)
                selected_element->model = &editor_models->models[editor_models->count-1];            
                        
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

void load_level_to_elements_array(const char* name, Array* load_elements){    

    File level_file;
    if( load_file(name, &level_file) == -1){
        
        printf("Level not found: %s\n",name);
    
    }

    load_level_elements_from_json(level_file.data , level_file.size_in_bytes , load_elements);

    close_file(&level_file);
}