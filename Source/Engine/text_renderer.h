#ifndef ENGINE_TEXT_RENDERER_H
#define ENGINE_TEXT_RENDERER_H

#include <ft2build.h>
#include <freetype/freetype.h>
#include <stdbool.h>
#include "../Engine/types.h"

typedef struct {
	u8 size;
	vec4 color;
}TextRenderData;

FT_Face face;


static inline void set_text_size(float size){
    FT_Set_Pixel_Sizes(face, 0, size);  
}

void text_renderer_update_pixel_size();
void text_renderer_init();

void text_render(const char *text, float x, float y, float sx, float sy , bool mark);

void text_render_in_screen_space( int text_size , const char* text, int x , int y);

void text_render_with_data();
#endif
