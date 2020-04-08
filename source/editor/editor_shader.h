/*Create 2019-09-09 by pavon */
#ifndef EDITOR_SHADER_H
#define EDITOR_SHADER_H

static const char*  skeletal_blue_joint_source = "#version 100 \n\
precision mediump float; \
    void main()\
    {\
        gl_FragColor = vec4(0,0,1,1);\
    }\
//end";



static const char*  editor_standard_fragment_shader_source = "#version 100 \n\
precision mediump float; \
   uniform sampler2D texture_sampler;\
   varying vec2 v_TexCoord;                     \
   uniform vec4 color; \
    void main()\
    {\
        gl_FragColor = texture2D(texture_sampler, v_TexCoord) * color;\
    }\
//end";


GLuint skeletal_blue_shader;
GLuint editor_standard_fragment_shader;

#endif // !EDITOR_SHADER_H