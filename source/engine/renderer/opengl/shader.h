//
// Created by pavon on 6/24/19.
//

#ifndef GLES2_SHADERS_H
#define GLES2_SHADERS_H

#include <GLES2/gl2.h>
#include <engine/utils.h>
#include <engine/log.h>
#include <engine/shaders_code.h>
#include <engine/model.h>

#define PE_SHADER_ERROR 9999999

typedef struct PEShaderCreation{
	PModel* model;
	GLuint vertex;
	GLuint pixel;
	GLuint* shader;
}PEShaderCreation;

GLuint compile_shader(const char* src , GLenum type);

GLuint create_engine_shader(GLuint vertex, GLuint fragment);

GLuint load_shader_file(const char* path, GLenum shader_type);

void pe_shader_create(PEShaderCreation* creation);

void pe_shader_new(PEShaderCreation* creation);

bool pe_shader_compile_std();

GLuint standart_vertex_shader;
GLuint standart_fragment_shader;
GLuint shader_standard_color;
GLuint shader_source_color_fragment_shader;
GLuint shader_skin_vertex; 
GLuint shader_skin_diffuse; 

GLuint actual_standard_fragment_shader;

static int shader_count = 0;

#endif 
