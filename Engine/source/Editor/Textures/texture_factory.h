#ifndef TEXTURES_RENDER_H
#define TEXTURES_RENDER_H

#include "../../model.h"

void init_UV_draw(Model* model);

void draw_UV();

void merge_textures();

void render_to_texture(int size);

void init_model_to_draw_texture();
#endif // !TEXTURES_RENDER_H