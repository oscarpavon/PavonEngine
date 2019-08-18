#include "text.h"

#include <stdio.h>
#include "../utils.h"
#include <GLES2/gl2.h>
#include "../Engine/shader.h"

#include <stdlib.h>
#include "../file_loader.h"
#include "editor.h"
#include "../gui.h"

#include "menus.h"

GLuint text_fragment_shader;
GLuint text_vertex_shader;

GLuint text_texture_id;
GLuint text_vertex_buffer_id;

GLuint text_shader_id;

GLuint uniform_text_color_location;

FT_GlyphSlot g;

const GLfloat black[4] = {1, 1, 1, 1};
const GLfloat red[4] = {1, 0, 0, 1};


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

void render_text_in_screen_space( int text_size , const char* text, int x , int y){
    float text_position_x = (-(camera_width_screen/2)+x) * pixel_size_x;    
    float text_position_y = (((camera_heigth_screen/2)-text_size)+y) * pixel_size_y;

    FT_Set_Pixel_Sizes(face, 0, text_size);
    render_text(text, text_position_x , text_position_y , pixel_size_x, pixel_size_y, false);  
}

unsigned short int directory_show_type = 50;

#include <dirent.h>
#include "ProjectManager/project_manager.h"

void list_directory_files(TextMenu *menu)
{

    struct dirent *de; // Pointer for directory entry

    char directory[sizeof(pavon_the_game_project_folder) + 30];
    memset(directory,0,sizeof(directory));
    sprintf(directory,"%s%s",pavon_the_game_project_folder,"/Content/");
    
    DIR *dr = opendir(directory);

    if (dr == NULL)
    {
        LOG("Could not open current directory\n");
    }

    int directory_count = 0;
    while ((de = readdir(dr)) != NULL)
    {
        directory_count++;
    }

    rewinddir(dr);
    int model_count = 0;
    int texture_count = 0;

    char directories_names[directory_count][30];
    memset(directories_names,0,sizeof(directories_names));
    char texture_names[directory_count][30];
    memset(texture_names,0,sizeof(texture_names));
    
    char model_names[directory_count][30];
    memset(model_names,0,sizeof(model_names));
    for (int i = 0; i < directory_count; i++)
    {
        de = readdir(dr);
        int name_lenght = strlen(de->d_name);
        for (int n = 0; n < name_lenght; n++)
        {
            if (de->d_name[n] == '.')
            {

                if (strcmp(&de->d_name[n + 1], "gltf") == 0)
                {
                    strcpy(&model_names[model_count][0], de->d_name);
                    model_count++;
                    continue;
                }
                if (strcmp(&de->d_name[n + 1], "glb") == 0)
                {
                    strcpy(&model_names[model_count][0], de->d_name);
                    model_count++;
                    continue;
                }
                if (strcmp(&de->d_name[n + 1], "blend") == 0)
                {
                    strcpy(&model_names[model_count][0], de->d_name);
                    model_count++;
                    continue;
                }
                if (strcmp(&de->d_name[n + 1], "jpg") == 0)
                {
                   strcpy(&texture_names[texture_count][0], de->d_name);
                    texture_count++;
                    continue;
                }
                if (strcmp(&de->d_name[n + 1], "png") == 0)
                {
                    strcpy(&texture_names[texture_count][0], de->d_name);
                    texture_count++;
                    continue;
                }
            
            }
        }
    }

    closedir(dr);

    int show_text_count = 0;
    if (directory_show_type == DIRECTORY_MODELS)
    {
        show_text_count = model_count;
        memcpy(directories_names, model_names, sizeof(model_names));
    }
    else if (directory_show_type == DIRECTORY_TEXTURES)
    {
        show_text_count = texture_count;
        memcpy(directories_names, texture_names, sizeof(texture_names));
    }
    menu->element_count = show_text_count;

    for (int i = 0; i < show_text_count; i++)
    {

        int y_pos = i * 20 + 20;
        if (i == 0)
        {
            y_pos = 20;
        }

        bool can_mark = false;
        if (menu->actual_element_select == i)
            can_mark = true;

        if (menu->actual_element_select == i)
        {
            strcpy(menu->text_for_action, &directories_names[i][0]);
        }

        if (menu->show){
            if(directories_names[i][0] != '\0')
            render_text(&directories_names[i][0], 0 + (-(camera_width_screen / 2)) * pixel_size_x, 1 - (y_pos)*pixel_size_y, pixel_size_x, pixel_size_y, can_mark);
        }
    }
}

void draw_element_text_list(TextMenu *menu, const char *text, int i)
{
    if (text == NULL)
        return;
    int y_pos = i * menu->text_size + menu->text_size;
    if (i == 0)
    {
        y_pos = menu->text_size;
    }
    bool can_mark = false;
    if (menu->actual_element_select == i)
    {
        can_mark = true;
        if (menu->element_selected)
        {
            strcpy(menu->text_for_action, text);
        }
    }

    render_text(text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 1 - (y_pos + 100) * pixel_size_y, pixel_size_x, pixel_size_y, can_mark);
}

void draw_directory_files()
{
    FT_Set_Pixel_Sizes(face, 0, 20);

    list_directory_files(&add_element_menu);
}

void draw_directory_file_type(unsigned short int type)
{
    FT_Set_Pixel_Sizes(face, 0, 20);
    directory_show_type = type;
    if (type == DIRECTORY_TEXTURES)
        list_directory_files(&menu_add_texture);
    else if (type == DIRECTORY_MODELS)
        list_directory_files(&add_element_menu);
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

    if (FT_New_Face(ft, "../NativeContent/Editor/DejaVuSerif.ttf", 0, &face))
    {
        LOG("Could not open font\n");
        return;
    }

    g = face->glyph;

    FT_Set_Pixel_Sizes(face, 0, 20);

    init_text_shader();
    create_text_texture_buffer();
    glGenBuffers(1, &text_vertex_buffer_id);

    pixel_size_x = 2.0 / camera_width_screen;
    pixel_size_y = 2.0 / camera_heigth_screen;

    mark_id = 0;

    menus_init();
}

void update_text_renderer_window_size()
{
    pixel_size_x = 2.0 / camera_width_screen;
    pixel_size_y = 2.0 / camera_heigth_screen;
}

void draw_editor_mode()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text(editor_mode_show_text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void draw_editor_sub_mode()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text(editor_sub_mode_text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 40) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void update_text_menu(TextMenu *menu)
{
    if (menu->execute)
    {
        if (menu->draw_text_funtion != NULL)
            menu->draw_text_funtion(menu);

        if (menu->type == MENU_TYPE_ADD_MODEL)
            draw_directory_file_type(DIRECTORY_MODELS);
        else if (menu->type == MENU_TYPE_ADD_TEXTURE)
            draw_directory_file_type(DIRECTORY_TEXTURES);

        if (menu->element_selected)
        {
            menu->execute = false;
            menu->show = false;
            menu->element_selected = false;

            if (menu->execute_function == NULL)
            {
                LOG("Menu execute function not assigned\n");
                return;
            }

            menu->execute_function(menu);
        }
    }
}
void draw_engine_memory()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text("Memory:", 0 + ((camera_width_screen / 2) - 400) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    char buf[5];
    float total_memory_in_kb = INIT_MEMORY / 1024;
    gcvt(total_memory_in_kb, 6, buf);
    render_text(buf, 0 + ((camera_width_screen / 2) - 340) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    render_text("/", 0 + ((camera_width_screen / 2) - 305) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);

    float free_memory = actual_free_memory / 1024;
    gcvt(free_memory, 6, buf);
    render_text(buf, 0 + ((camera_width_screen / 2) - 300) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    render_text("kb", 0 + ((camera_width_screen / 2) - 265) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void draw_frame_time()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    render_text("Frame:", 0 + ((camera_width_screen / 2) - 200) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    char buf[5];

    gcvt(frame_time, 6, buf);
    render_text(buf, 0 + ((camera_width_screen / 2) - 150) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    render_text("ms", 0 + ((camera_width_screen / 2) - 140) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

#include "commands.h"
void text_renderer_loop()
{
    if (editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
    {
        set_text_size(12);
        render_text(command_text_buffer, 0 + (-(camera_width_screen / 2)) * pixel_size_x, 0 + (-(camera_heigth_screen / 2) + 24) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    }

    draw_engine_memory();
    draw_editor_mode();
    draw_frame_time();

    if (editor_sub_mode != EDITOR_SUB_MODE_NULL)
    {
        draw_editor_sub_mode();
    }

    if (editor_mode == EDITOR_DEFAULT_MODE && editor_sub_mode != EDITOR_SUB_MODE_TEXT_INPUT)
    {
        can_open_text_menu_with_key(&add_element_menu, &input.A, GLFW_MOD_SHIFT);
        can_open_text_menu_with_key(&menu_editor_element_list, &input.L, NULL);
        can_open_text_menu_with_key(&menu_add_texture, &input.T, GLFW_MOD_SHIFT);

        can_open_text_menu_with_key(&menu_add_native_editor_element, &input.E, GLFW_MOD_SHIFT);
        update_text_menu(&menu_add_texture);

        update_text_menu(&add_element_menu);

        update_text_menu(&menu_editor_element_list);

        update_text_menu(&menu_add_native_editor_element);

        update_text_menu(&menu_show_gui_elements);
    }

    if (editor_mode == EDITOR_MODE_GUI_EDITOR)
        can_open_text_menu_with_key(&menu_show_gui_elements, &input.L, NULL);

    draw_menus();
}