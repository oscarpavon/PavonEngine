//
// Created by pavon on 6/24/19.
//

#ifndef ENGINE_SHADERS_H
#define ENGINE_SHADERS_H

#include <GLES2/gl2.h>
#include "utils.h"
#include "log.h"
#include "shaders_code.h"

GLuint standart_vertex_shader;
GLuint standart_fragment_shader;
GLuint shader_standard_color;
GLuint shader_source_color_fragment_shader;

GLuint actual_standard_fragment_shader;

GLuint compile_shader(const char* src , GLenum type);

static int shader_count = 0;
GLuint static inline create_engine_shader(GLuint vertex, GLuint fragment){
    shader_count++;
    GLuint new_shader = glCreateProgram();
    glAttachShader(new_shader, vertex);
    glAttachShader(new_shader, fragment);
    glLinkProgram(new_shader);
    return new_shader;
}

GLuint load_shader_file(const char* path, GLenum shader_type);

void shader_compile_standard_shaders();
#endif 
