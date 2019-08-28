
#include "engine.h"
#include "text_renderer.h"

GLuint text_fragment_shader;
GLuint text_vertex_shader;

GLuint text_texture_id;
GLuint text_vertex_buffer_id;

GLuint text_shader_id;

FT_GlyphSlot g;

GLuint uniform_text_color_location;


void render_text(const char *text, float x, float y, float pixel_size_x, float pixel_size_y, bool mark)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    glUseProgram(text_shader_id);

    GLint uniform_color = get_uniform_location(text_shader_id, "color");

    const char *p;

    for (p = text; *p; p++)
    {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
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
            g->bitmap.buffer);

        check_error("text texture");

        float x2 = x + g->bitmap_left * pixel_size_x;
        float y2 = -y - g->bitmap_top * pixel_size_y;
        float w = g->bitmap.width * pixel_size_x;
        float h = g->bitmap.rows * pixel_size_y;

        GLfloat box[4][4] = {
            {x2, -y2, 0, 0},
            {x2 + w, -y2, 1, 0},
            {x2, -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        if (mark)
            glUniform4fv(uniform_color, 1, red);
        else
            glUniform4fv(uniform_color, 1, black);
        check_error("send text color");

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, text_vertex_buffer_id);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (g->advance.x / 64) * pixel_size_x;
        y += (g->advance.y / 64) * pixel_size_y;

        check_error("texture error");
    }
    glEnable(GL_CULL_FACE);
}

void init_text_shader()
{
    text_fragment_shader = compile_shader(fragment_shader_colorized, GL_FRAGMENT_SHADER);
    text_vertex_shader = compile_shader(text_vertex_shader_source, GL_VERTEX_SHADER);

    text_shader_id = create_engine_shader(text_vertex_shader, text_fragment_shader);

    

}


void init_text_renderer()
{
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        LOG("Could not init freetype library\n");
        return;
    }

    File font;
    load_file("../NativeContent/DejaVuSerif.ttf",&font);

    int error = FT_New_Memory_Face(ft,font.data,font.size_in_bytes,0,&face);

    if(error !=  0 ){
        LOG("Could not open font\n");
        return;
    }

    //close_file(&font);
   /*  if (FT_New_Face(ft, "../NativeContent/DejaVuSerif.ttf", 0, &face))
    {
        LOG("Could not open font\n");
        return;
    } */

    g = face->glyph;

    FT_Set_Pixel_Sizes(face, 0, 20);

    init_text_shader();
    create_text_texture_buffer();
    glGenBuffers(1, &text_vertex_buffer_id);

    pixel_size_x = 2.0 / camera_width_screen;
    pixel_size_y = 2.0 / camera_heigth_screen;

    
}

void update_text_renderer_window_size()
{
    pixel_size_x = 2.0 / camera_width_screen;
    pixel_size_y = 2.0 / camera_heigth_screen;
}


void create_text_texture_buffer()
{

    glGenTextures(1, &text_texture_id);
    glBindTexture(GL_TEXTURE_2D, text_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}