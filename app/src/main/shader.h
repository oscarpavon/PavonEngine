//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_SHADER_H
#define PAVON_MOBILE_SHADER_H

#include <GLES2/gl2.h>
#include "utils.h"

static const char* triVertShader = "#version 100 \n \
attribute vec3 vPosition;\
attribute vec2 inUV;\
varying vec2 v_TexCoord; \
uniform mat4 MVP;   \
void main()\
{\
    gl_Position = MVP * vec4(vPosition,1);\
    v_TexCoord = inUV;\
}\
//end";

static const char*  triFragShader = "#version 100 \n\
precision mediump float; \
   uniform sampler2D texture_sampler;\
   varying vec2 v_TexCoord;                     \
    void main()\
    {\
        gl_FragColor = texture2D(texture_sampler, v_TexCoord);\
    }\
//end";


static const char*  text_fragment_shader_source = "#version 100 \n\
    precision mediump float;                    \
   uniform sampler2D texture_sampler;           \
   varying vec2 v_TexCoord;                     \
   uniform vec4 color;                     \
    void main()\
    {\
        gl_FragColor = vec4(1, 1, 1, texture2D(texture_sampler, v_TexCoord).r) * color;\
    }\
//end";

static const char* text_vertex_shader_source = "#version 100 \n \
attribute vec4 coord;\
varying vec2 v_TexCoord; \
void main()\
{\
    gl_Position = vec4(coord.xy, 0, 1);\
    v_TexCoord = coord.zw;\
}\
//end";


GLuint standart_vertex_shader;
GLuint standart_fragment_shader;

GLuint compile_shader(const char* src , GLenum type);

#endif //PAVON_MOBILE_SHADER_H
