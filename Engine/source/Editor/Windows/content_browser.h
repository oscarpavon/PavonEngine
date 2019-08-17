#ifndef EDITOR_WINDOW_CONTENT_H
#define EDITOR_WINDOW_CONTENT_H

#include "../windows_manager.h"

EditorWindow window_content_browser;

void editor_window_content_init();

void editor_window_content_browser_draw();


typedef struct ContentView{
    bool new_update;
    u32 thumbnail_image_id;
    vec2 position;
    char thumbnail_image_path[30];
    char content_name[30];
}ContentView;
#endif