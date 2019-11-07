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
int file_explorer_previous_directory_count;

bool file_have_extension(const char* file_name, const char* extension){

        int name_lenght = strlen(file_name);
        for (int n = name_lenght; n > 0; n--)
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

void file_explorer_set_file_extension_color(const char* name, vec4 color){
	if(file_have_extension(name,"pb"))
		memcpy(color,(vec4){0.2,1,0.5,1},sizeof(vec4));
	else if(file_have_extension(name,"png"))
		memcpy(color,(vec4){1,0.5,1,1},sizeof(vec4));
	else if(file_have_extension(name,"jpg"))
		memcpy(color,(vec4){0.2,1,1,1},sizeof(vec4));
}

bool file_explorer_valid_directory(const char* path){
    struct dirent *directory_data; 
    DIR *directory_pointer = opendir(path);
    
	if (directory_pointer == NULL)
		return false;

	return true;
}

void file_manager_list_directory(const char* path){
	
    struct dirent *directory_data; // Pointer for directory entry
    DIR *directory_pointer = opendir(path);

    if (directory_pointer == NULL)
    {
        LOG("Could not open current directory\n");
		
		file_explorer_can_open_directory = false;
		return;
    }
	file_manager_current_directory_count = 0; 
    while ((directory_data = readdir(directory_pointer)) != NULL)
    {
        file_manager_current_directory_count++;
    }

    rewinddir(directory_pointer);
	int draw_count = 0;	
	int directory_count = 0;
	int files_count = 0;
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
							memcpy(text_data.color,(vec4){1,0,1,1},sizeof(vec4));
							break;
							directory_count++;
						}
			case DT_LNK:{
							memcpy(text_data.color,(vec4){0,1,0.2f,1},sizeof(vec4));
							break;
						}
		}

		file_explorer_set_file_extension_color(directory_data->d_name,text_data.color);

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

void file_explorer_get_absolute_path(){
		strcpy(file_manager_back_directory,file_manager_current_path);
		char new_directory[300];
		memset(new_directory,0,strlen(new_directory));
		strcat(new_directory,file_manager_current_path);	
		strcat(new_directory,file_manager_temp_path);	
		if(file_explorer_can_open_directory)
			strcat(new_directory,"/");	
		if(file_explorer_valid_directory(new_directory)){	
			memset(file_manager_current_path,0,sizeof(file_manager_current_path));
			strcpy(file_manager_current_path,new_directory);
		}
}

void file_explorer_back_directory(){
        int name_lenght = strlen(file_manager_current_path);
		if(name_lenght == 1)
			return;
		int letter_count_to_slash = 0;
		int slash_count = 0;
        for (int n = name_lenght; n > 0; n--)
        {
            if (file_manager_current_path[n] != '/')
            {
				letter_count_to_slash++;
			}else
			{
				slash_count++;
				if(slash_count >=2){
					break;
				}
			}	
		}
		file_manager_current_path[name_lenght-letter_count_to_slash] = '\0';		
		LOG("Back directory: %s \n",file_manager_current_path);
}

void file_explorer_enter(){
		file_explorer_get_absolute_path();
	file_explorer_previous_directory_count = file_manager_current_directory_id;	
		file_manager_current_directory_id = 1;
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
		if(file_explorer_can_open_directory){
			file_explorer_enter();
		}
	}

	if(key_released(&input.H)){
		file_explorer_back_directory();		
		file_manager_current_directory_id = file_explorer_previous_directory_count;
	}	
}

void file_explorer_update(){
	file_explorer_input();
	file_manager_list_directory(file_manager_current_path);

		TextRenderData text_data;
		text_data.size = 12;
		text_data.position[0] = 0;		
		text_data.position[1] = 0;
		memcpy(text_data.color,(vec4){1,1,1,1},sizeof(vec4));
		text_render_in_screen_space_with_data(file_manager_current_path,&text_data);
}

void file_explorer_update_commands(){


}
