#include "content_manager.h"
#include "editor.h"
#include "Serialization/json_writer.h"
#include "ProjectManager/project_manager.h"
#include <string.h>

void content_manager_serialize_static_mesh_values(){
    new_text_token("name","Content Name");
}

void content_manager_serialize_static_mesh(){    
    new_element(content_manager_serialize_static_mesh_values);
}

void content_manager_create_engine_binary(const char* name){
    FILE* content_brute_file = fopen(name,"rb");
    fseek(content_brute_file, 0L, SEEK_END);
    u32 file_size = ftell(content_brute_file);    

    char glb_path[strlen(name) + 4];
    sprintf(glb_path,"%s%s",name,".pb");
    FILE* engine_binary = fopen(glb_path,"wb");

    fprintf(engine_binary,"pvnB");
    u32 version = 1;
    fwrite(&version,sizeof(u32),1,engine_binary);

    int size = file_size + 12;
    fwrite(&size,sizeof(u32),1,engine_binary);


    fwrite(&file_size,sizeof(u32),1,engine_binary);// + engine file JSON
    ContentType type = CONTENT_TYPE_STATIC_MESH;
    fwrite(&type,sizeof(u32),1,engine_binary);
    unsigned char brute_file[file_size];
    rewind(content_brute_file);
    for (u32 i = 0; i < file_size; i++)
    {
        brute_file[i]  =  fgetc(content_brute_file);
    }
    fwrite(brute_file,sizeof(unsigned char),file_size,engine_binary);

    fclose(engine_binary);
    fclose(content_brute_file);
}

void content_manager_create_static_mesh(const char* path){
    Content new_content;
    memset(&new_content,0,sizeof(Content));
    char new_path[strlen(pavon_the_game_project_folder)+40];
    sprintf(new_path,"%s%s%s",pavon_the_game_project_folder,"Content/content",".pvnf");
    serializer_serialize_data(new_path,content_manager_serialize_static_mesh);
    content_manager_create_engine_binary(path);
}

void content_manager_create_engine_content_type(const char* path, ContentType type){
    switch (type)
    {
    case CONTENT_TYPE_STATIC_MESH:
        {
            content_manager_create_static_mesh(path);
        }
        break;
    
    default:
        break;
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
                content_manager_create_engine_content_type(path,CONTENT_TYPE_STATIC_MESH);
                continue;
            }

        
        }
    }
    

}

void content_manager_init(){
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
                    //content_manager_create_engine_content_type(de->d_name,CONTENT_TYPE_STATIC_MESH);
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


}
