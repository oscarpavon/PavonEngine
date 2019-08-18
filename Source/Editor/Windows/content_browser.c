#include "content_browser.h"
#include "../../engine.h"
#include "../Textures/texture_factory.h"
#include "../text.h"
#include <dirent.h>

#include "../../gui.h"

#include "../ProjectManager/project_manager.h"
#include "../editor_shader.h"
#include "../editor.h"
#include "../commands.h"

Model content_model;

Array array_content_views;
Array array_finding_content;

void editor_window_content_browser_draw_content_view(ContentView* content_view){
    glDisable(GL_CULL_FACE);

    glUseProgram(content_view->shader_id);
    glBindTexture(GL_TEXTURE_2D,content_view->thumbnail_image_id);

    two_dimension_screen_space_send_matrix(content_view->shader_id, content_view->size, content_view->position);

    glBindBuffer(GL_ARRAY_BUFFER,UI_plane_vertex_buffer_id);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

    send_color_to_shader(content_view->shader_id,(vec4){1,1,1,1});
    if(content_view->selected){
        send_color_to_shader(content_view->shader_id,(vec4){1,0,0,1});            
    }

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);        

    check_error("content thumbnail");

    render_text_in_screen_space(content_view->text_size,content_view->content_name,content_view->position[0]-64,-content_view->position[1]-64);

}
void content_find_per_name(const char* name){
    for (int i = 0; i < array_content_views.count; i++)
    {
        bool found = false;
        ContentView* content_view = get_from_array(&array_content_views,i);

        if(content_view->content_name[0] != name[0])
            continue;

        for (u8 j = 0; j < strlen(name); j++)
        {
            if(content_view->content_name[j] != name[j]){
                found = false;
                break;
            }

            found = true;
        }
        if(found)
        add_to_array(&array_finding_content,&content_view);
    }
}


void editor_window_content_browser_search_mode(){
    clean_array(&array_finding_content);
    content_find_per_name(&command_text_buffer[1]);

    for (int i = 0; i < array_finding_content.count; i++)
    {
        ContentView** ppContent_view = get_from_array(&array_finding_content,i);
        ContentView* content_view = ppContent_view[0];
        editor_window_content_browser_draw_content_view(content_view);
    }
}

void editor_window_content_browser_draw(){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(0.1,0.2,0.4,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(editor_search_objects){
        editor_window_content_browser_search_mode();
        
    }else{
        ContentView* mark_content = get_from_array(&array_content_views,0);
        mark_content->selected = true;
        
        for (int i = 0; i < array_content_views.count; i++)
        {
            ContentView* content_view = get_from_array(&array_content_views,i);
            if(!content_view)
                continue;
            editor_window_content_browser_draw_content_view(content_view);
        }
    
    }    

    if (editor_sub_mode == EDITOR_SUB_MODE_TEXT_INPUT)
    {
        set_text_size(12);
        render_text(command_text_buffer, 0 + (-(camera_width_screen / 2)) * pixel_size_x, 0 + (-(camera_heigth_screen / 2) + 24) * pixel_size_y, pixel_size_x, pixel_size_y, false);
    }


    glfwSwapBuffers(window_content_browser.window);
}


ContentView first;

void content_view_create_model_view(int image_size){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(1,0.2,0.4,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Model* model = get_from_array(actual_model_array,0);
    if(model){
        draw_simgle_model(model);
    }
    
}





void create_contents_view(){    
    render_to_texture(128,content_view_create_model_view);
    char directory[sizeof(pavon_the_game_project_folder) + 30];
    memset(directory,0,sizeof(directory));
    sprintf(directory,"%s%s%s%i%s",pavon_the_game_project_folder,".thumbnails/","content",24,".jpg");
    texture_current_export_name = directory;
    texture_export(128);    
}

void editor_window_content_get_models_path(){
    
    struct dirent *de; // Pointer for directory entry

    char directory[sizeof(pavon_the_game_project_folder) + 30];
    memset(directory,0,sizeof(directory));
    sprintf(directory,"%s%s",pavon_the_game_project_folder,"/Content/");
    DIR *dr = opendir(directory);

    if (dr == NULL)
    {
        LOG("Could not open current directory\n");
        return;
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

    init_array(&array_finding_content,sizeof(ContentView*),(model_count+texture_count));
    init_array(&array_content_views,sizeof(ContentView),(model_count+texture_count));
    int max_x = camera_width_screen / 128;
    int last_x = 64;
    int last_y = 64;
    int object_x_count = 0;
    for (int i = 0; i < model_count; i++)
    {
        ContentView new_content_view;
        memset(&new_content_view,0,sizeof(ContentView));
        strcpy(new_content_view.content_name,model_names[i]);

        Texture new_texture;
        memset(&new_texture,0,sizeof(Texture));
        new_texture.image = load_image("/home/pavon/PavonTheGame/.thumbnails/content24.jpg");
        load_texture_to_GPU(&new_texture); 

        new_content_view.thumbnail_image_id = new_texture.id;
        new_content_view.text_size = 12;

        new_content_view.shader_id = create_engine_shader(standart_vertex_shader,editor_standard_fragment_shader);
        new_content_view.size[0] = 58;
        new_content_view.size[1] = 58;
        if(object_x_count < max_x){            
            if(object_x_count != 0)
                last_x += 128;
            object_x_count++;
        }else{
            last_y += 148;
            object_x_count = 1;
            last_x = 64;
        }
        new_content_view.position[0] = last_x;
        new_content_view.position[1] = last_y;

        new_content_view.pixel_size = 64 + 12;
        add_to_array(&array_content_views,&new_content_view);

    }
    
}

void editor_window_content_init(){
    glfwMakeContextCurrent(window_content_browser.window);

    glfwSetKeyCallback(window_content_browser.window, key_callback);
	glfwSetCursorPosCallback(window_content_browser.window, mouse_callback);
	glfwSetMouseButtonCallback(window_content_browser.window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_content_browser.window, window_resize_callback);
    glfwSetCharCallback(window_content_browser.window, character_callback);
    glfwSetWindowFocusCallback(window_content_browser.window,window_focus_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    create_contents_view();

    editor_window_content_get_models_path();    

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    ContentView* content_view = get_from_array(&array_content_views,0);
    
}

