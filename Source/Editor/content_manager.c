#include "content_manager.h"
#include "editor.h"
#include "Serialization/json_writer.h"
#include "ProjectManager/project_manager.h"
#include <string.h>
#include "Windows/content_browser.h"
#include "Textures/texture_factory.h"

void content_manager_serialize_static_mesh_values(){
    new_text_token("name","Content Name");
}

void content_manager_serialize_static_mesh(){    
    new_element(content_manager_serialize_static_mesh_values);
}

void content_manager_create_engine_binary(const char* name, ContentType type){
    FILE* content_brute_file;
    if(type == CONTENT_TYPE_STATIC_MESH){

        content_brute_file= fopen(name,"rb");
    }else{
        content_brute_file= fopen(name,"r");
    }

    fseek(content_brute_file, 0L, SEEK_END);
    u32 file_size = ftell(content_brute_file);    

    char glb_path[strlen(name) + 4];
    sprintf(glb_path,"%s",name);
    sprintf(&glb_path[strlen(glb_path)-4],"%s",".pb");
    FILE* engine_binary = fopen(glb_path,"wb");

    fprintf(engine_binary,"pvnB");
    u32 version = 1;
    fwrite(&version,sizeof(u32),1,engine_binary);

    int size = file_size + 12;
    fwrite(&size,sizeof(u32),1,engine_binary);


    fwrite(&file_size,sizeof(u32),1,engine_binary);// + engine file JSON
    ContentType new_content_type = type;
    fwrite(&type,sizeof(u32),1,engine_binary);
    unsigned char brute_file[file_size];
    rewind(content_brute_file);
    for (u32 i = 0; i < file_size; i++)
    {
        brute_file[i]  =  fgetc(content_brute_file);
    }
    fwrite(brute_file,sizeof(unsigned char),file_size,engine_binary);

    content_create_thumbnail(name,CONTENT_TYPE_TEXTURE);
    TextureCreated created_texture = texture_create_to_memory(1,128);
    u32 thumnail_size = (u32)created_texture.size;
    fwrite(&thumnail_size,sizeof(u32),1,engine_binary);
    u32 thumnail_type = 35;
    fwrite(&thumnail_type,sizeof(u32),1,engine_binary);
    fwrite(created_texture.data,created_texture.size,1,engine_binary);

    fclose(engine_binary);
    fclose(content_brute_file);

    //remove(name);

    if(type == CONTENT_TYPE_STATIC_MESH){//FIXME: create function where import and add to viewport
        //content_manager_load_content(glb_path);
        //editor_init_new_added_element();
    }

}


void content_manager_import(const char* path){
    int name_lenght = strlen(path);
    for (int n = 0; n < name_lenght; n++)
    {
        if (path[n] == '.')
        {
            if (strcmp(&path[n + 1], "glb") == 0)
            {
                content_manager_create_engine_binary(path,CONTENT_TYPE_STATIC_MESH);
                continue;
            }     
            if (strcmp(&path[n + 1], "png") == 0)
            {   
                
                content_manager_create_engine_binary(path,CONTENT_TYPE_TEXTURE);
                continue;
            }
            if (strcmp(&path[n + 1], "pb") == 0)
            { 
                continue;
            }
            LOG("File not support: %s \n",path);
        }
        
    }    
}

void content_manager_init(){
     struct dirent *de; // Pointer for directory entry

    char directory[sizeof(pavon_the_game_project_folder) + 60];
    memset(directory,0,sizeof(directory));
    sprintf(directory,"%s%s",pavon_the_game_project_folder,"Content/");
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

    for (int i = 0; i < directory_count; i++)
    {
        de = readdir(dr);
        sprintf(directory,"%s%s%s",pavon_the_game_project_folder,"Content/",de->d_name);

        content_manager_import(directory);
    }

    closedir(dr);


}
