#include "text.h"

#include <stdio.h>
#include "../utils.h"
#include <GLES2/gl2.h>
#include "../Engine/shader.h"

#include <stdlib.h>
#include "../Engine/file_loader.h"
#include "editor.h"
#include "../Engine/gui.h"

#include "menus.h"

#include "commands.h"

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
            text_render(&directories_names[i][0], 0 + (-(camera_width_screen / 2)) * pixel_size_x, 1 - (y_pos)*pixel_size_y, pixel_size_x, pixel_size_y, can_mark);
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

    text_render(text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 1 - (y_pos + 100) * pixel_size_y, pixel_size_x, pixel_size_y, can_mark);
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


void editor_text_init(){
    mark_id = 0;

    menus_init();
}


void draw_editor_mode()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    text_render(editor_mode_show_text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void draw_editor_sub_mode()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    text_render(editor_sub_mode_text, 0 + ((camera_width_screen / 2) - 100) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 40) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}


void draw_engine_memory()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    text_render("Memory:", 0 + ((camera_width_screen / 2) - 400) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    char buf[5];
    float total_memory_in_kb = INIT_MEMORY / 1024;
    gcvt(total_memory_in_kb, 6, buf);
    text_render(buf, 0 + ((camera_width_screen / 2) - 340) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    text_render("/", 0 + ((camera_width_screen / 2) - 305) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);

    float free_memory = actual_free_memory / 1024;
    gcvt(free_memory, 6, buf);
    text_render(buf, 0 + ((camera_width_screen / 2) - 300) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    text_render("kb", 0 + ((camera_width_screen / 2) - 265) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void draw_frame_time()
{
    FT_Set_Pixel_Sizes(face, 0, 12);
    text_render("Frame:", 0 + ((camera_width_screen / 2) - 200) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    char buf[16];
    memset(buf,0,sizeof(buf));

    gcvt(frame_time, 6, buf);
    text_render(buf, 0 + ((camera_width_screen / 2) - 850) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    text_render("ms", 0 + ((camera_width_screen / 2) - 800) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
}

void draw_FPS(){
    FT_Set_Pixel_Sizes(face, 0, 12);
    text_render("FPS:", 0 + ((camera_width_screen / 2) - 1100) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    char buf[16];
    memset(buf,0,sizeof(buf));

    gcvt(FPS, 6, buf);
    text_render(buf, 0 + ((camera_width_screen / 2) - 1000) * pixel_size_x, 0 + ((camera_heigth_screen / 2) - 20) * pixel_size_y, pixel_size_x, pixel_size_y, false);

}

void text_renderer_loop()
{
    if (editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
    {
        set_text_size(12);
        text_render(command_text_buffer, 0 + (-(camera_width_screen / 2)) * pixel_size_x, 0 + (-(camera_heigth_screen / 2) + 24) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    }

    draw_engine_memory();
    draw_editor_mode();
    draw_frame_time();
    draw_FPS();

    if (editor_sub_mode != EDITOR_SUB_MODE_NULL)
    {
        draw_editor_sub_mode();
    }

    if (editor_mode == EDITOR_DEFAULT_MODE && editor_sub_mode != EDITOR_SUB_MODE_TEXT_INPUT && window_editor_main.focus)
    {
        //can_open_text_menu_with_key(&add_element_menu, &input.A, GLFW_MOD_SHIFT);
        can_open_text_menu_with_key(&menu_editor_element_list, &input.L, NULL);
        can_open_text_menu_with_key(&menu_add_texture, &input.T, GLFW_MOD_SHIFT);

        can_open_text_menu_with_key(&menu_add_native_editor_element, &input.E, GLFW_MOD_SHIFT);
        text_menu_update(&menu_add_texture);

        text_menu_update(&add_element_menu);

        text_menu_update(&menu_editor_element_list);

        text_menu_update(&menu_add_native_editor_element);

        text_menu_update(&menu_show_gui_elements);
    }

    if (editor_mode == EDITOR_MODE_GUI_EDITOR)
        can_open_text_menu_with_key(&menu_show_gui_elements, &input.L, NULL);

    draw_menus();
}
