#ifndef EDITOR_TEXT_H
#define EDITOR_TEXT_H

#include "../Engine/text_renderer.h"

#include <stdbool.h>
#include "../Engine/array.h"

#include "input.h"

#define DIRECTORY_MODELS 0
#define DIRECTORY_TEXTURES 1


void text_renderer_init();

void draw_directory_files();
void draw_directory_file_type(unsigned short int type);

void text_renderer_loop();

typedef enum{
    MENU_TYPE_ADD_MODEL,
    MENU_TYPE_ADD_TEXTURE    
}TextMenuType;

typedef struct TextMenu TextMenu;

typedef void(*TextMenuFunction)(TextMenu*);

typedef struct TextMenu{
    char name[30];
    bool show;
    bool element_selected;
    bool execute;
    unsigned int actual_element_select;
    TextMenuFunction execute_function;
    TextMenuFunction draw_text_funtion;
    TextMenuType type;
    unsigned short int element_count;
    unsigned short int text_size;
    char text_for_action[30];
    Key* open_key;
    int mods_key;
}TextMenu;


int mark_id;

Array menus;

#endif