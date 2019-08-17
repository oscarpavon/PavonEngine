#include "content_browser.h"
#include "../../engine.h"
#include "../Textures/texture_factory.h"
#include "../text.h"
#include <dirent.h>

#include "../../gui.h"
#include "../../engine.h"

Model content_model;

Array array_content_views;

void editor_window_content_browser_draw(){
    glfwMakeContextCurrent(window_content_browser.window);

    glClearColor(0.1,0.2,0.4,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Model* model = get_from_array(actual_model_array,0);
    if(model){
        draw_simgle_model(model);
    }
   
    //draw_engine_memory();
    
    
    for (int i = 0; i < array_content_views.count; i++)
    {
        ContentView* content_view = get_from_array(&array_content_views,i);
        if(!content_view)
            continue;
        
        glDisable(GL_CULL_FACE);

        glUseProgram(content_view->shader_id);
        glBindTexture(GL_TEXTURE_2D,content_view->thumbnail_image_id);

        two_dimension_screen_space_send_matrix(content_view->shader_id, content_view->size, content_view->position);

        glBindBuffer(GL_ARRAY_BUFFER,UI_plane_vertex_buffer_id);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        check_error("logo");

        render_text_in_screen_space(12,content_view->content_name,128*i,-128);
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
    int max_x = camera_width_screen / 128;
    int last_x = 64;
    int last_y = 64;
    for (int i = 0; i < model_count; i++)
    {
        ContentView new_content_view;
        memset(&new_content_view,0,sizeof(ContentView));
        strcpy(new_content_view.content_name,model_names[i]);
        Texture new_texture;
        memset(&new_texture,0,sizeof(Texture));
        new_texture.image = load_image("../../Project/.thumbnails/first.png");
        load_texture_to_GPU(&new_texture); 

        new_content_view.thumbnail_image_id = new_texture.id;

        new_content_view.shader_id = create_engine_shader(standart_vertex_shader,standart_fragment_shader);
        new_content_view.size[0] = 58;
        new_content_view.size[1] = 58;
        if(i*128 < max_x){
            last_x = i*128;

        }else{
            last_y += 128;
            max_x = 128;
        }
        new_content_view.position[0] = (last_y);
        new_content_view.position[1] = last_x+64;
        add_to_array(&array_content_views,&new_content_view);

    }
    
}

void editor_window_content_init(){
    glfwMakeContextCurrent(window_content_browser.window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //create_contents_view();

    editor_window_content_get_models_path();    

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    ContentView* content_view = get_from_array(&array_content_views,0);
    
}

