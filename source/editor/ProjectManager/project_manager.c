#include "project_manager.h"
#include "../editor.h"
#include "../text.h"
#include "../menus.h"
#include "../file_explorer.h"
#include "../content_manager.h"

TextMenuType project_manager_menu;

TextMenu project_select_menu;

int project_manager_open(const char* name){
	LOG("Open project with path: %s\n", name);

	
	char project_file_path[500];
	memset(project_file_path,0,sizeof(project_file_path));
	strcat(project_file_path,projects_folder);
	strcat(project_file_path,name);
	memset(project_manager_current_path,0,sizeof(project_manager_current_path));
	strcat(project_manager_current_path,project_file_path);	
	
	
	strcat(project_file_path,"/project.pb");

    ContentType type = content_manager_load_content(project_file_path);		
	if(type == CONTENT_TYPE_NULL){
		LOG("Project directory not valid\n");	
		return -1;
	}

	memset(project_manager_current_project_name,0,sizeof(project_manager_current_project_name));//TODO: save and load project name
	strcpy(project_manager_current_project_name,"Opened project");
	project_manager_can_show = false;
	return 0;
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

	memset(project_manager_current_project_name,0,sizeof(project_manager_current_project_name));
	strcpy(project_manager_current_project_name,name);
	
	content_manager_create_engine_binary(project_manager_current_path,CONTENT_TYPE_PROJECT);
}

void project_manager_save(EditorProject* project){

}

void project_manager_export(){
	
}


void project_manager_window_draw(){
	text_render_in_screen_space(40,"Pavon Engine - Editor",0,0);
	text_menu_update(&project_select_menu);	
}

	
void project_manager_menu_select_project_draw(TextMenu* menu){
		char projects_names[30][30];
		memset(projects_names,0,sizeof(projects_names));
		directory_get_only_directories(projects_folder,projects_names);	
    float text_size = 12;
    set_text_size(text_size);
    menu->text_size  = text_size;
    menu->element_count = 5;
    for(int i = 0; i < 4 ; i++){

        char* name = projects_names[i];
        
        draw_element_text_list(menu,name,i);
    }

} 

void project_manager_window_input(){

	if(key_released(&input.ENTER))
		project_manager_can_show = false;
}

void project_manager_update(){
	project_manager_window_draw();
	project_manager_window_input();
}

void project_manager_init() {
  project_manager_can_show = true;

  memset(&project_select_menu, 0, sizeof(TextMenu));
  project_select_menu.execute_function = NULL;
  project_select_menu.draw_text_funtion =
      &project_manager_menu_select_project_draw;
  project_select_menu.execute = true;
  project_select_menu.show = true;
  project_select_menu.menu_in_editor = true;
  project_select_menu.open_key = NULL;
  project_select_menu.mods_key = NULL;
  project_select_menu.element_count = 0;
}
