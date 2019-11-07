#include "project_manager.h"
#include "../editor.h"
void project_manager_new(const char* name){


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

