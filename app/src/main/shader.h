//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_SHADER_H
#define PAVON_MOBILE_SHADER_H

#include <GLES2/gl2.h>
#include "utils.h"

static const char* triVertShader = "attribute vec3 vPosition;\
attribute vec2 inUV;\
varying vec2 v_TexCoord; \
uniform mat4 MVP;   \
void main()\
{\
    gl_Position = MVP * vec4(vPosition,1);\
    v_TexCoord = inUV;\
}\
//end";

static const char*  triFragShader = "precision mediump float; \
   uniform sampler2D texture_sampler;\
   varying vec2 v_TexCoord;                     \
    void main()\
    {\
        gl_FragColor = texture2D(texture_sampler, v_TexCoord);\
    }\
//end";

GLuint compile_shader(const char* src , GLenum type);

#endif //PAVON_MOBILE_SHADER_H
