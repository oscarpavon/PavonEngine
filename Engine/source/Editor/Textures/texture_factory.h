#ifndef TEXTURES_RENDER_H
#define TEXTURES_RENDER_H

#include "../../model.h"

const char* current_texture_name;

void init_UV_draw(Model* model);

void draw_UV();

void merge_textures(const char* name);

void render_to_texture(int size);

void init_model_to_draw_texture();

void scale_UV(float size, Model* model, vec2 UV_offset);

void translate_UV(vec3 tranlation, Model* model, vec2 UV_offset);
#endif // !TEXTURES_RENDER_H