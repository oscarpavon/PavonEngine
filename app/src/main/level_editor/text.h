#ifndef EDITOR_TEXT_H
#define EDITOR_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H

void init_text_renderer();
void render_text(const char *text, float x, float y, float sx, float sy);

#endif