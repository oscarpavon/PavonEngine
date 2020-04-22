#include "level.h"
#include "editor.h"
#include <engine/level.h>
void editor_level_new(const char * name){
	LOG("New level\n");

}

void editor_level_save(const char* path){
	
	content_manager_create_engine_binary(path,CONTENT_TYPE_LEVEL);
}


void editor_level_open(const char* name){
    strcpy(opened_file_name,name);

    struct timespec time1, time2;
    int temp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

    char new_file_name_with_path[strlen(pavon_the_game_project_folder) + 40];
		sprintf(new_file_name_with_path,"%s%s%s.lvl",pavon_the_game_project_folder,"Content/levels/",name);

    int level_result = load_level_to_elements_array(name, actual_elements_array);
    if(level_result != 0)
        return;   
    
    actual_model_array = &array_models_loaded;
    for(int i = 0; i< texts.count ; i++){
        char* model_path = array_get(&texts,i);
        load_model(model_path);
    }    
    actual_model_array = &editor_models;

    for(int i = 0; i< textures_paths.count ; i++){
				memset(new_file_name_with_path,0,sizeof(new_file_name_with_path));
                                sprintf(new_file_name_with_path, "%s%s%s",
                                        pavon_the_game_project_folder,
                                        "Content/",
                                        (char*)array_get(&textures_paths, i));
                                load_simple_image(new_file_name_with_path);
    } 

    for_each_element_components(&init_element_component);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec result = time_diffence(time1,time2);
    long millisecond = result.tv_nsec / 1000000;
    LOG("Level loading time: %ld ms\n",millisecond);
} 
