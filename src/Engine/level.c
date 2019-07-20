#include "level.h"

#include "../model.h"

#include "../engine.h"

#ifdef EDITOR
#include "../level_editor/editor.h"
#endif // EDITOR

#include "../file_loader.h"

#define CAMERA__ELEMENT_ID 300

void add_loaded_elements(Array* load_elements, Array* editor_models, Array* editor_elements){
    init_array(editor_models,sizeof(Model),load_elements->count);
    for(int i = 0; i < load_elements->count; i++){
        Element* element = get_from_array(load_elements,i);
        switch (element->type)
        {
        case ELEMENT_TYPE_PLAYER_START:{
            new_empty_model();
            new_empty_element();
            selected_element->type = element->type;
            selected_model->draw = false;
            strcpy(selected_element->name, "PlayerStart01");            
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
            player1 = selected_element;
            continue;
            break;
        }
        
        default:
            break;
        }       

    }
}

int load_level_to_elements_array(const char* name, Array* load_elements){    

    File level_file;
    if( load_file(name, &level_file) == -1){
        
        LOG("Level not found: %s\n",name);
        return -1;
    }

    load_level_elements_from_json(level_file.data , level_file.size_in_bytes , load_elements);

    close_file(&level_file);
    return 0;
}