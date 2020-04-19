#ifndef EDITOR_TEXT_H
#define EDITOR_TEXT_H

#include "../engine/text_renderer.h"

#include <stdbool.h>
#include "../engine/array.h"

#include "input.h"
#define DIRECTORY_MODELS 0
#define DIRECTORY_TEXTURES 1



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
	bool always_open;
	bool menu_in_editor;
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

void draw_directory_files();
void draw_directory_file_type(unsigned short int type);

void draw_element_text_list(TextMenu *menu, const char *text, int i);

void text_renderer_loop();

void text_draw_commands();

int mark_id;

void editor_text_init();
Array menus;

#endif
