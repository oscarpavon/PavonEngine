#ifndef ENGINE_TEXT_RENDERER_H
#define ENGINE_TEXT_RENDERER_H

#include <ft2build.h>
#include <freetype/freetype.h>

void text_renderer_update_pixel_size();
void text_renderer_init();

FT_Face face;

static const float black[4] = {1, 1, 1, 1};
static const float red[4] = {1, 0, 0, 1};

static inline void set_text_size(float size){
    FT_Set_Pixel_Sizes(face, 0, size);  
}

#endif