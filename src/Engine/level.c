#include "level.h"

#include "../model.h"
#include "../camera.h"
#include "../engine.h"
#ifdef EDITOR
#include "../level_editor/editor.h"
#endif // EDITOR

#define CAMERA__ELEMENT_ID 300

void add_loaded_elements(Array* load_elements, ModelArray* editor_models, Array* editor_elements){
    init_model_array(editor_models,load_elements->count);
    for(int i = 0; i < load_elements->count; i++){
        Element* element = &load_elements->data[i*load_elements->element_bytes_size];
        if(element->type == ELEMENT_TYPE_PLAYER_START){
            new_empty_model();
            new_empty_element();
            selected_model->draw = false;
            strcpy(selected_element->name, "PlayerStart01");
            selected_element->type = ELEMENT_TYPE_PLAYER_START;
            glm_vec3_copy(element->position,selected_element->position);
            glm_vec4_copy(element->rotation,selected_element->rotation);
            glm_translate(selected_model->model_mat,selected_element->position);
            player_start = selected_element;
            continue;
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
                
            add_editor_element(element->model_path);        
            add_editor_texture(element->texture_path);

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

    load_level_elements_from_json(file_buffer,file_size, load_elements);

    
}