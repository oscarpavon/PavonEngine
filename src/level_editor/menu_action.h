#include "text.h"

#include <stdio.h>
#include "../utils.h"
#include <GLES2/gl2.h>
#include "../shader.h"
#include "../camera.h"

#include <stdlib.h>
#include "../file_loader.h"
#include "editor.h"
#include "../gui.h"

void can_open_text_menu_with_key(TextMenu* menu, Key* open_key, int mods){
    if(mods == -1){
        if(input.SHIFT.pressed)
            return;
        if( key_released(open_key) ){
            menu->execute = true;
            menu->show = true;
        }
    }else{
        if( key_released(open_key) ){
            if(mods == open_key->mods){
                menu->execute = true;
                menu->show = true;
            }            
        }
    }    

    if(menu->show){
        if(key_released(&input.ESC)){
            menu->execute = false;
            menu->show = false;
        }
        if(key_released(&input.J)){
            menu->actual_element_select++;
        }
        if(key_released(&input.K)){
            menu->actual_element_select--;
        }
        if(key_released(&input.ENTER)){
            menu->element_selected = true;
            menu->show = false;
        }

    }
}

void new_text_menu(const char* name, Key* open_key, int mods_key, 
                    TextMenuFunction draw_function, 
                    TextMenuFunction execute_function){
    TextMenu menu;
    memset(&menu,0,sizeof(TextMenu));
    menu.execute_function = execute_function;
    menu.draw_text_funtion = draw_function;
    menu.execute = false;
    menu.show = false;
    menu.open_key = open_key;
    menu.mods_key = mods_key;
    strcpy(menu.name,name);
    add_to_array(&menus,&menu);
}

const char* components_names[4] = {"Camera Component", "Sphere Component", "Cube Component", "Transform Component"};

void draw_available_components(TextMenu* menu){
    float text_size = 12;
    set_text_size(text_size);
    menu->text_size  = text_size;
    for(int i = 0; i < 4 ; i++){

        char* name = components_names[i];
        
        draw_element_text_list(menu,name,i);
    }
}

void draw_components_from_selected_element(TextMenu* menu){
    float text_size = 12;
    set_text_size(text_size);
    menu->text_size  = text_size;
    if(selected_element == NULL)
        return;
    if(selected_element->components_count== 0){
        draw_element_text_list(menu,"No Components",0);
    }

    for(int i = 0; i < selected_element->components_count ; i++){

        char* name = "Component ";
        
        draw_element_text_list(menu,name,i);
    }
}

void menu_action_add_component_to_select_element(TextMenu* menu){
    if( strcmp("Transform Component", menu->text_for_action) == 0 ){ 
        TransformComponent transform;
        init_transfrom_component(&transform);
        add_component_to_selected_element(sizeof(TransformComponent),&transform,TRASNFORM_COMPONENT);

    } 
}

void init_menus(){
    init_array(&menus,sizeof(TextMenu),10);
    new_text_menu("Element Component List",&input.C, -1,  &draw_components_from_selected_element, NULL);
    new_text_menu("Add Component",&input.C, GLFW_MOD_SHIFT,  &draw_available_components, &menu_action_add_component_to_select_element);

}

void draw_menus(){
    TextMenu* menus_list = get_from_array(&menus,0);
    for(int i = 0; i < menus.count ; i++){
        can_open_text_menu_with_key(&menus_list[i],menus_list[i].open_key,menus_list[i].mods_key);
        update_text_menu(&menus_list[i]);
    }
}

void menu_action_add_element(TextMenu* menu){
    add_element_with_model_path(menu->text_for_action);
}

void menu_action_add_texture_to_element(TextMenu* menu){
    add_texture_to_selected_element_with_image_path(menu->text_for_action);
}

void menu_action_select_element(TextMenu* menu){
    LOG("Selected element\n");
    int id = menu->actual_element_select;
    Element* element = NULL;
   
    if(!input.SHIFT.pressed){
        if(selected_element != NULL)
            selected_element->selected = false;
    }else{
        add_to_array(&selected_elements_id,&selected_element->id);
    }   

    element = get_from_array(&editor_elements,id);
    element->selected = true;
    selected_element = element;
    if(input.SHIFT.pressed){
        add_to_array(&selected_elements_id,&selected_element->id);
    }

    for(int i = 0 ; i < selected_elements_id.count ; i++){
        unsigned short int *id = get_from_array(&selected_elements_id,i);
        unsigned short int id_number;
        memcpy(&id_number,id,sizeof(unsigned short int));
        LOG("seleteted: %i\n", id_number);
    }
}

void menu_action_add_editor_native_element(TextMenu* menu){
    add_editor_native_element(menu->text_for_action);
    LOG("Add editor native element: %s\n",menu->text_for_action);
}
#define EDITOR_NATIVE_ELEMETN_COUNT 8
const char* elements_names[EDITOR_NATIVE_ELEMETN_COUNT] = { 
    "Empty Element" , "Camera" , "Player Start", "Collider" , "Sphere" , "Cube" , "Cyllinder" , "Floor"
    };

void menu_action_draw_native_editor_elments(TextMenu* menu){
    float text_size = 12;
    set_text_size(text_size);
    menu->text_size = text_size;   

    for(int i = 0; i < EDITOR_NATIVE_ELEMETN_COUNT; i++){
        
        const char* name = elements_names[i];
        draw_element_text_list(menu,name,i);
       
    }
    
}

void menu_action_draw_editor_elements(TextMenu* menu){
    float text_size = 12;
    set_text_size(text_size);
    menu->text_size  = text_size;
    if(editor_elements.count == 0){
        draw_element_text_list(menu,"No Elements",0);
    }

    for(int i = 0; i < editor_elements.count ; i++){
        Element* element = (Element*)get_from_array(&editor_elements,i);
        char* name = element->name;
        if( strcmp(element->name,"") == 0){
            if(strcmp(element->name,"") == 0)
                name = "no_name";
            else
                name = element->name;
        }
        draw_element_text_list(menu,name,i);
    }
       
}
void menu_action_select_gui_element(TextMenu* menu){
    selected_button = get_from_array(actual_buttons_array,menu->actual_element_select);    
}

void menu_action_draw_gui_elements(TextMenu* menu){
    menu->text_size = 12;
    for (int i = 0; i < actual_buttons_array->count ; i++)
    {
        Button* button = get_from_array(actual_buttons_array,i);
        draw_element_text_list(menu,button->name,i);
    }
}