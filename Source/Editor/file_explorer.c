#include "file_explorer.h"
#include "../Engine/text_renderer.h"
#include "../Engine/log.h"
#include "../Engine/text_renderer.h"
#include "../Editor/input.h"
#include <dirent.h>
#include "content_manager.h"

int file_manager_current_directory_count = 0;
int file_manager_current_directory_id = 0;
char file_manager_current_path[200] = "/";
char file_manager_temp_path[200];
char file_manager_back_directory[200];
bool file_explorer_can_open_directory;


void file_manager_list_directory(const char* path){
	
    struct dirent *directory_data; // Pointer for directory entry
    DIR *directory_pointer = opendir(path);

    if (directory_pointer == NULL)
    {
        LOG("Could not open current directory\n");
		return;
    }
	file_manager_current_directory_count = 0;
	
    while ((directory_data = readdir(directory_pointer)) != NULL)
    {
        file_manager_current_directory_count++;
    }

    rewinddir(directory_pointer);
	int draw_count = 0;	
    for (int i = 0; i < file_manager_current_directory_count ; i++)
    {
        directory_data = readdir(directory_pointer);
		if(directory_data->d_name[0] == '.')
			continue;
		draw_count++;	
		
		TextRenderData text_data;
		text_data.size = 12;
		text_data.position[0] = 0;		
		text_data.position[1] = draw_count*-14;
		memcpy(text_data.color,(vec4){1,1,1,1},sizeof(vec4));

		switch(directory_data->d_type){
			case DT_DIR:{
							memcpy(text_data.color,(vec4){0,1,0.2f,1},sizeof(vec4));
							break;
						}
			case DT_LNK:{
							memcpy(text_data.color,(vec4){1,0,1,1},sizeof(vec4));
							break;
						}

		}
		
		if(file_manager_current_directory_id == draw_count){	
			memcpy(file_manager_temp_path,directory_data->d_name,sizeof(directory_data->d_name));
			memcpy(text_data.color,(vec4){1,1,0.2f,1},sizeof(vec4));
			if(directory_data->d_type == DT_DIR){
				file_explorer_can_open_directory = true;
			}else
				file_explorer_can_open_directory = false;

		}	
		text_render_in_screen_space_with_data(directory_data->d_name,&text_data);
	}

    closedir(directory_pointer);
}

bool file_have_extension(const char* file_name, const char* extension){

        int name_lenght = strlen(file_name);
        for (int n = 0; n < name_lenght; n++)
        {
            if (file_name[n] == '.')
            {

                if (strcmp(&file_name[n + 1], extension) == 0)
                {
					return true;
                }else
					break;
			}
		}

	return false;
}

void file_explorer_get_absolute_path(){

		strcpy(file_manager_back_directory,file_manager_current_path);
		char new_directory[300];
		memset(new_directory,0,strlen(new_directory));
		strcat(new_directory,file_manager_current_path);	
		strcat(new_directory,file_manager_temp_path);	
		if(file_explorer_can_open_directory)
			strcat(new_directory,"/");	
		memset(file_manager_current_path,0,sizeof(file_manager_current_path));
		strcpy(file_manager_current_path,new_directory);
}

void file_explorer_enter(){
		file_explorer_get_absolute_path();
		file_manager_current_directory_id = 0;
		LOG("%s\n",file_manager_current_path);	
}

void file_explorer_input(){
	if(key_released(&input.J)){
		file_manager_current_directory_id++;	
	
	}	
	if(key_released(&input.K)){
			file_manager_current_directory_id--;	
	}

	if(key_released(&input.ENTER)){
		if(file_explorer_can_open_directory)
			file_explorer_enter();
		else{
			if(file_have_extension(file_manager_temp_path,"pb"))	
			{
				LOG("pavon file extension detected\n");
				file_explorer_get_absolute_path();
				LOG("Open file: %s \n",file_manager_current_path);

	ContentType type = content_manager_load_content(file_manager_current_path);
    switch (type)
    {
    case CONTENT_TYPE_STATIC_MESH:{
        
		editor_init_new_added_element();
        break;
    }
   	case CONTENT_TYPE_TEXTURE:{
		break;


	} 
    default:
        break;
    }

			}
		}

	}

	if(key_released(&input.L)){
		file_explorer_enter();
	}

	if(key_released(&input.H)){
		strcpy(file_manager_current_path,file_manager_back_directory);
		file_manager_current_directory_id = 0;
		LOG("Back\n");
	}	
}

void file_explorer_update(){
	file_explorer_input();
	file_manager_list_directory(file_manager_current_path);
}
