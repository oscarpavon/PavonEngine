#include "project_manager.h"
#include "../editor.h"
#include "../text.h"
#include "../menu.h"
#include "../file_explorer.h"
TextMenuType project_manager_menu;

void project_manager_open(){
	
}

void project_manager_new(const char* name){
	LOG("New project command function called, project name: %s \n",name);
	char new_path[300];
	memset(new_path,0,sizeof(new_path));
	strcat(new_path,projects_folder);
	strcat(new_path,"/");
	strcat(new_path,name);
	LOG("New full path %s\n",new_path);

	memcpy(project_manager_current_path,new_path,strlen(new_path));
	project_manager_current_path[strlen(new_path)] = '\0';	
	LOG("New project project path: %s \n",project_manager_current_path);
	mkdir(new_path,0700);

	strcat(new_path,content_folder);
	mkdir(new_path,0700);
}

void project_manager_save(EditorProject* project){

}

void project_manager_export(){

}

void project_manager_draw_projects_list(){


}

void project_manager_window_draw(){
	text_render_in_screen_space(40,"Pavon Editor",0,0);

}

void project_manager_window_input(){

	if(key_released(&input.ENTER))
		project_manager_can_show = false;
}
void project_manager_update(){
	project_manager_window_draw();
	project_manager_window_input();
}
void project_manager_init(){
	project_manager_can_show = true;
	
}

