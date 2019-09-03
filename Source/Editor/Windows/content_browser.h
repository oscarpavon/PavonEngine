#ifndef EDITOR_WINDOW_CONTENT_H
#define EDITOR_WINDOW_CONTENT_H

#include "windows_manager.h"

#include "../../Engine/content_manager.h"

EditorWindow window_content_browser;

void editor_window_content_init();

void editor_window_content_browser_draw();

void editor_window_content_browser_input_update();

void content_create_thumbnail(const char * brute_content_path,ContentType type);

void editor_window_content_browser_update();

void editor_window_content_browser_close_window();
typedef struct ContentView{
    bool new_update;
    DrawData draw;    
    vec2 position;
    vec2 size;
    char thumbnail_image_path[100];
    char content_name[100];   
    bool selected;
    u8 pixel_size;//thumbnail + text size in pixel
    u8 text_size;
    u8 thumbnail_image_size;
}ContentView;



bool editor_window_content_browser_hint;

#endif
