#ifndef ENGINE_SHADERS_CODE
#define ENGINE_SHADERS_CODE
	

static const char* pe_shader_src_std_vert= "#version 100 \n \
attribute vec3 vPosition;\
attribute vec2 inUV;\
attribute vec3 vNormal;\
uniform mat4 MVP;   \
uniform mat4 model; \
varying vec3 FragPosition;\
varying vec2 v_TexCoord; \
varying vec3 Normal;\
void main()\
{\
    gl_PointSize = 10.0;\
    Normal = vNormal;\
    v_TexCoord = inUV;\
    FragPosition = vec3(model * vec4(vPosition,1));\
    gl_Position = MVP * vec4(vPosition,1);\
}\
//end";


static const char*  pe_shader_src_std_frag_texture= "#version 100 \n\
precision mediump float; \
   uniform sampler2D texture_sampler;\
   varying vec2 v_TexCoord;                     \
    void main()\
    {\
        gl_FragColor = texture2D(texture_sampler, v_TexCoord);\
    }\
//end";


static const char*  pe_shader_src_std_frag = "#version 100 \n\
    precision mediump float;                    \
   uniform sampler2D texture_sampler;           \
   varying vec2 v_TexCoord;                     \
   uniform vec4 color;                     \
    void main()\
    {\
        gl_FragColor = vec4(1, 1, 1, texture2D(texture_sampler, v_TexCoord).r) * color;\
    }\
//end";

static const char*  pe_shader_src_color = "#version 100 \n\
    precision mediump float;                    \
   uniform vec4 color;                     \
    void main()\
    {\
        gl_FragColor = color;\
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

#endif
