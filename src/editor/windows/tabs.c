#include "tabs.h"
#include "../../editor/editor.h"
void tabs_new(EngineWindow* window, const char* name){
	if(!window->tabs.initialized){
		array_init(&window->tabs,sizeof(EditorTab),20);
	}
	EditorTab new_tab;
	memset(&new_tab,0,sizeof(EditorTab));
	strcpy(new_tab.name,name);
	array_add(&window->tabs,&new_tab);	
	window->tab_current = array_pop(&window->tabs);	
}

void tabs_next_in_window(EngineWindow* window){

	window->tab_current_id++;
	window->tab_current = array_get(&window->tabs,window->tab_current_id);	

}
void tabs_previous_in_window(EngineWindow* window){
	window->tab_current_id--;
	window->tab_current = array_get(&window->tabs,window->tab_current_id);	

}

void tabs_draw_tabs_bar(EngineWindow* window){
for(int i = 0 ; i < window->tabs.count ; i++){
	EditorTab* tab = array_get(&window->tabs,i);	
	text_render_in_screen_space(12,tab->name,i*40,0);
			
}
}
