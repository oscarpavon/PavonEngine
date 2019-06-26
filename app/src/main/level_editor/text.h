#ifndef EDITOR_TEXT_H
#define EDITOR_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdbool.h>
#include "../vector.h"

void init_text_renderer();
void render_text(const char *text, float x, float y, float sx, float sy , bool mark);
void draw_directory_files();

void text_renderer_loop();
bool draw_text_menu;

typedef struct{
    bool selected;
    const char* text;    
}TextElement;

typedef struct {
    int count;    
    TextElement *elements;
}TextColumn;

 int mark_id;
 float open_file;

#endif