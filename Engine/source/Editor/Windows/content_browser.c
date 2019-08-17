#include "content_browser.h"
#include "../../engine.h"
#include "../Textures/texture_factory.h"
#include "../text.h"
#include <dirent.h>

Model content_model;

Array array_content_views;

GLuint other_text_texture;

void editor_window_content_browser_draw(){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(0.1,0.2,0.4,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Model* model = get_from_array(actual_model_array,0);
    if(model){
        draw_simgle_model(model);
    }
   
    //draw_engine_memory();
    ContentView* content_view = get_from_array(&array_content_views,0);
    if(content_view){
        render_text_in_screen_space(12,content_view->content_name,0,0);
    }
    
    glfwSwapBuffers(window_content_browser.window);
}


ContentView first;

void content_view_create_model_view(int image_size){
    editor_window_content_browser_draw();

}

void create_contents_view(){    
    render_to_texture(128,content_view_create_model_view);
    texture_current_export_name = "../../Project/.thumbnails/first.png";
    texture_export(128);    
}

void editor_window_content_get_models_path(){
    struct dirent *de; // Pointer for directory entry

    DIR *dr = opendir("../assets/");

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

    init_array(&array_content_views,sizeof(ContentView),(model_count+texture_count));
    for (int i = 0; i < model_count; i++)
    {
        ContentView new_content_view;
        memset(&new_content_view,0,sizeof(ContentView));
        strcpy(new_content_view.content_name,model_names[i]);
        add_to_array(&array_content_views,&new_content_view);

    }
    
}

void editor_window_content_init(){
    glfwMakeContextCurrent(window_content_browser.window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    create_contents_view();

    editor_window_content_get_models_path();

    
    glGenTextures(1, &other_text_texture);
    glBindTexture(GL_TEXTURE_2D, other_text_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

