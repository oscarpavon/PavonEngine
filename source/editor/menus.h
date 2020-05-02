#ifndef EDITOR_MENUS_H
#define EDITOR_MENUS_H

#include "text.h"
void menus_init();


PETextMenu add_element_menu;
PETextMenu menu_add_texture;
PETextMenu menu_editor_element_list;

PETextMenu menu_add_native_editor_element;

PETextMenu menu_show_gui_elements;

void menu_new(const char* name, Key* open_key, int mods_key, 
                    PETextMenuFunction draw_function, 
                    PETextMenuFunction execute_function);

void menu_new_from_data(const char* name, PETextMenu* new_menu);
void menu_can_open_with_key(PETextMenu* menu, Key* open_key, int mods);

void text_menu_update(PETextMenu *menu);

void menu_draw_menus();
void pe_editor_menus_update();

Array menus;
#endif
