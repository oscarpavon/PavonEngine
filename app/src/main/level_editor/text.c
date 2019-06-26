#include "text.h"

#include <stdio.h>
#include "../utils.h"
#include <GLES2/gl2.h>
#include "../shader.h"

GLuint text_fragment_shader;
GLuint text_vertex_shader;

GLuint text_texture_id;
GLuint text_vertex_buffer_id;

GLuint text_shader_id;

GLuint uniform_text_color_location;

FT_Face face;
FT_GlyphSlot g;

void render_text(const char *text, float x, float y, float sx, float sy) {
  //glEnable(GL_BLEND);  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 

  const char *p;

  for(p = text; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;
 
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_ALPHA,
      g->bitmap.width,
      g->bitmap.rows,
      0,
      GL_ALPHA,
      GL_UNSIGNED_BYTE,
      g->bitmap.buffer
    );
 
    float x2 = x + g->bitmap_left * sx;
    float y2 = -y - g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;
 
    GLfloat box[4][4] = {
        {x2,     -y2    , 0, 0},
        {x2 + w, -y2    , 1, 0},
        {x2,     -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };
    
    glUseProgram(text_shader_id);

    glBindTexture(GL_TEXTURE_2D, text_texture_id);  

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, text_vertex_buffer_id);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
    x += (g->advance.x/64) * sx;
    y += (g->advance.y/64) * sy;

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("text draw error \n");
        LOGW("Error %08x \n",error);
    }

  }
  glDisable(GL_BLEND);
}

void create_text_texture_buffer(){

    glGenTextures(1, &text_texture_id);
    glBindTexture(GL_TEXTURE_2D, text_texture_id);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

}

void init_text_shader(){
    text_fragment_shader = compile_shader(text_fragment_shader_source,GL_FRAGMENT_SHADER);
    text_vertex_shader = compile_shader(text_vertex_shader_source,GL_VERTEX_SHADER);

    text_shader_id = glCreateProgram();
    glAttachShader(text_shader_id, text_vertex_shader);
    glAttachShader(text_shader_id, text_fragment_shader);
    glLinkProgram(text_shader_id);

    glGenBuffers(1, &text_vertex_buffer_id);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, text_vertex_buffer_id);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    //uniform_text_color_location =  glGetUniformLocation(text_shader_id,"color");

}

void init_text_renderer(){
    FT_Library ft;

    if(FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init freetype library\n");

    }    

    if(FT_New_Face(ft, "DejaVuSerif.ttf", 0, &face)) {
        fprintf(stderr, "Could not open font\n");    
    }  
    

    g = face->glyph;

    FT_Set_Pixel_Sizes(face, 0, 48);

    init_text_shader();
    create_text_texture_buffer();
}