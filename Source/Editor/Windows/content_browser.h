#ifndef EDITOR_WINDOW_CONTENT_H
#define EDITOR_WINDOW_CONTENT_H

#include "../windows_manager.h"

EditorWindow window_content_browser;

void editor_window_content_init();

void editor_window_content_browser_draw();

void editor_window_content_browser_input_update();

typedef struct ContentView{
    bool new_update;
    u32 thumbnail_image_id;    
    vec2 position;
    vec2 size;
    char thumbnail_image_path[30];
    char content_name[30];
    u32 shader_id;
    bool selected;
    u8 pixel_size;//thumbnail + text size in pixel
    u8 text_size;
    u8 thumbnail_image_size;
}ContentView;

bool editor_window_content_browser_hint;

#endif