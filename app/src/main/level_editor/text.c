#include "text.h"

#include <stdio.h>
#include "../utils.h"
#include <GLES2/gl2.h>
#include "../shader.h"
#include "../camera.h"

#include <stdlib.h>
#include "../file_loader.h"
#include "editor.h"

GLuint text_fragment_shader;
GLuint text_vertex_shader;

GLuint text_texture_id;
GLuint text_vertex_buffer_id;

GLuint text_shader_id;

GLuint uniform_text_color_location;

FT_Face face;
FT_GlyphSlot g;

TextColumn* dir_text_column;

float pixel_size_x;
float pixel_size_y;

void render_text(const char *text, float x, float y, float sx, float sy, bool mark) {
    glEnable(GL_BLEND);  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    glUseProgram(text_shader_id);
    GLfloat black[4] = {0, 0, 0, 1};
    GLfloat red[4] = {1, 0, 0, 1};
    GLint uniform_color =  glGetUniformLocation(text_shader_id,"color");

  const char *p;

  for(p = text; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;
    
    glBindTexture(GL_TEXTURE_2D, text_texture_id); 

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_LUMINANCE,
      g->bitmap.width,
      g->bitmap.rows,
      0,
      GL_LUMINANCE,
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
    
    if(mark)
        glUniform4fv(uniform_color, 1, red);
    else
        glUniform4fv(uniform_color, 1, black);

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
  //glDisable(GL_BLEND);
   
    
}
#include <dirent.h> 
void list_directory_files(TextColumn* column){   
   
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("../assets/"); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory\n" );         
    }   

    int directory_count = 0;
    while ((de = readdir(dr)) != NULL) {    
        directory_count++; 
    }

    rewinddir(dr);
    
       
    for(int i = 0; i < directory_count ; i++){        
        de = readdir(dr);
        
        int y_pos = i*20+20;
        if(i == 0){
            y_pos = 20;
        }
        bool can_mark = false;
        if(mark_id == i)
            can_mark = true;

        if(open_file == 5 && mark_id == i){
            char* editor_path = "../assets/";
            char* buffer = malloc(500);
            memset(buffer,0,500);
            strcat(buffer,editor_path);
            strcat(buffer,de->d_name);
            add_element(de->d_name);
            //printf("%s\n",buffer);
            free(buffer);
        }
           

        render_text(de->d_name,  0,   1 - (y_pos) * pixel_size_y, pixel_size_x, pixel_size_y, can_mark);   
    }    
  
    closedir(dr);   
   
}


void draw_directory_files(){
    FT_Set_Pixel_Sizes(face, 0, 20);    
   
    list_directory_files(dir_text_column);
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

    FT_Set_Pixel_Sizes(face, 0, 20);

    init_text_shader();
    create_text_texture_buffer();
    
        
    mark_id = 0;
    open_file = 0;

    add_element_menu.show = false;
    add_element_menu.element_selected = false;

    pixel_size_x = 2.0 / camera_width_screen;
    pixel_size_y = 2.0 / camera_heigth_screen;
}

void draw_editor_mode(){
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text(editor_mode_show_text , 0 + ((camera_width_screen/2)-100) * pixel_size_x , 0 + ((camera_heigth_screen/2)-20) * pixel_size_y  , pixel_size_x, pixel_size_y, false);  
}

void text_renderer_loop(){ 

    
    draw_editor_mode();

    if(add_element_menu.show){       
       
        draw_directory_files();
        if(add_element_menu.element_selected){
            add_element_menu.show = false;
            add_element_menu.element_selected = false;
        }
       
    }
}