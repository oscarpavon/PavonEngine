#ifndef ENGINE_TEXT_RENDERER_H
#define ENGINE_TEXT_RENDERER_H

#include <ft2build.h>
#include <freetype/freetype.h>

void update_text_renderer_window_size();
void init_text_renderer();

FT_Face face;

static const float black[4] = {1, 1, 1, 1};
static const float red[4] = {1, 0, 0, 1};

static inline void set_text_size(float size){
    FT_Set_Pixel_Sizes(face, 0, size);  
}

#endif