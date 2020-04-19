#ifndef ENGINE_TEXT_RENDERER_H
#define ENGINE_TEXT_RENDERER_H

#include <stdbool.h>
#include "../engine/types.h"

typedef struct PRenderThreadDefinition{
	void(*init)(void);
	void(*draw)(void);
	void(*end)(void);
}PRenderThreadDefinition;

typedef struct {
	u8 size;
	vec4 color;
	vec2 position;
}TextRenderData;

typedef struct FT_FaceRec_*  FT_Face;

FT_Face face;


void text_renderer_update_pixel_size();

void pe_text_set_size(int size);

void text_renderer_update_pixel_size();
void text_renderer_init();

void text_render_with_data(const char *text, float x, float y, float pixel_size_x, float pixel_size_y, vec4 color);

void text_render_in_screen_space_with_data( const char* text, TextRenderData* data);

void text_render(const char *text, float x, float y, float sx, float sy , bool mark);

void text_render_in_screen_space( int text_size , const char* text, int x , int y);

static inline void set_text_size(float size){
	pe_text_set_size(size);
}
PRenderThreadDefinition render_thread_definition;

#endif
