#ifndef EDITOR_MENUS_H
#define EDITOR_MENUS_H

#include "text.h"
void menus_init();


TextMenu add_element_menu;
TextMenu menu_add_texture;
TextMenu menu_editor_element_list;

TextMenu menu_add_native_editor_element;

TextMenu menu_show_gui_elements;

void menu_new(const char* name, Key* open_key, int mods_key, 
                    TextMenuFunction draw_function, 
                    TextMenuFunction execute_function);

void menu_new_from_data(const char* name, TextMenu* new_menu);
void menu_can_open_with_key(TextMenu* menu, Key* open_key, int mods);

void text_menu_update(TextMenu *menu);

void menu_draw_menus();
void pe_editor_menus_update();
#endif
