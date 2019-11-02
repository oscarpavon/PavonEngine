#include "file_explorer.h"
#include "../Engine/text_renderer.h"
#include "../Engine/log.h"
#include "../Engine/text_renderer.h"
#include "../Editor/input.h"
#include <dirent.h>

void file_manager_list_directory(const char* path){
	
    struct dirent *directory_data; // Pointer for directory entry
    DIR *directory_pointer = opendir(path);

    if (directory_pointer == NULL)
    {
        LOG("Could not open current directory\n");
    }

    int directory_count = 0;
	
    while ((directory_data = readdir(directory_pointer)) != NULL)
    {
        directory_count++;
    }

    rewinddir(directory_pointer);
	
    for (int i = 0; i < directory_count; i++)
    {
        directory_data = readdir(directory_pointer);
		
		text_render_in_screen_space(12,directory_data->d_name,0,i*-14);
	}

    closedir(directory_pointer);
}
void file_explorer_input(){
		if(key_released(&input.H)){
		//back	
		}	

}
void file_explorer_update(){
	file_explorer_input();
	file_manager_list_directory("./");
	/*	text_render_in_screen_space(12,"test1",0,0);
		text_render_in_screen_space(12,"test2",0,-40);
		text_render_in_screen_space(12,"test3",10,0);
		text_render_in_screen_space(12,"test3",50,0);
*/
}
