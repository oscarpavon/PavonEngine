#ifndef EDITOR_WINDOW_CONTENT_H
#define EDITOR_WINDOW_CONTENT_H

#include "../windows_manager.h"

EditorWindow window_content_browser;

void editor_window_content_init();

void editor_window_content_browser_draw();

void editor_window_content_browser_input_update();

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

typedef enum ContentType{
    CONTENT_TYPE_STATIC_MESH,
    CONTENT_TYPE_SKINNED_MESH,
    CONTENT_TYPE_TEXTURE,
    CONTENT_TYPE_SOUND,
    CONTENT_TYPE_SHADER,
    CONTENT_TYPE_MATERIAL
}ContentType;

typedef struct Content{
    ContentType type;
    char path[100];
    
}Content;

bool editor_window_content_browser_hint;

#endif