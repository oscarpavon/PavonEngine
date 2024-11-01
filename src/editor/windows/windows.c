
#include "../editor.h"
#include "../text.h"
#include "content_browser.h"
#include "tabs.h"


void editor_windows_init_data(){
	EngineWindow content_browser_window;
	memset(&content_browser_window,0,sizeof(EngineWindow));
	content_browser_window.init = editor_window_content_init;
	content_browser_window.draw = editor_window_content_browser_draw;
	content_browser_window.input = editor_window_content_browser_input_update;
	content_browser_window.finish = editor_window_content_browser_close_window;
	array_add(&engine_windows,&content_browser_window);
	window_content_browser = array_pop(&engine_windows);	

}

