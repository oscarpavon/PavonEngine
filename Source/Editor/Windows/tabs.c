#include "tabs.h"
#include "../../Editor/editor.h"
void tabs_new(EditorWindow* window, const char* name){
	if(!window->tabs.initialized){
		array_init(&window->tabs,sizeof(EditorTab),20);
	}
	EditorTab new_tab;
	memset(&new_tab,0,sizeof(EditorTab));
	strcpy(new_tab.name,name);
	array_add(&window->tabs,&new_tab);	
	window->tab_current = array_pop(&window->tabs);	
}

void tabs_next_in_window(EditorWindow* window){

	window->tab_current_id++;
	window->tab_current = array_get(&window->tabs,window->tab_current_id);	

}
void tabs_previous_in_window(EditorWindow* window){
	window->tab_current_id--;
	window->tab_current = array_get(&window->tabs,window->tab_current_id);	

}

void tabs_draw_tabs_bar(EditorWindow* window){
	text_render_in_screen_space(12,"Tab01",0,0);
	text_render_in_screen_space(12,"Tab2",40,0);
			

}
