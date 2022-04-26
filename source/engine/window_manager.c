#include "windows_manager.h"
#include <engine/log.h>

void window_manager_update_windows_input(){
	
  	//Draw tab bar 	& draw current tabb 
	for(u8 i = 0; i<engine_windows.count ; i++ ){
		EngineWindow* window = array_get(&engine_windows,i);
		if(!window->initialized)
			   continue;
		
        //The mouse need to stay in the window for window->input call	
		if(window->focus){
			if(window->input)
				window->input();
		}
	}
}



bool pe_wm_should_close(EngineWindow* window){

    return false;
}


void windows_manager_init(){

}

void pe_input_init(){


}

void window_update_viewport(int width, int height){
		glViewport(0,0,width,height);
#ifdef LINUX
    text_renderer_update_pixel_size();
#endif
		camera_update_aspect_ratio(&current_window->camera);
}

void window_create(EngineWindow *win, EngineWindow* share_window, const char* name){
    if(win == NULL){
        //LOG("ERROR: Window not found\n");
        return;
    }
    if(win->initialized)
        return;

    current_window = win;
    
   //// GLFWwindow* share_glfw_window = NULL;
    //if(share_window)
     //share_glfw_window = share_window->window;

	
	if (pe_renderer_type == PEWMVULKAN) {
		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  	}

	//GLFWwindow* new_window = glfwCreateWindow( INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y,name, NULL ,share_glfw_window );
	//if(!new_window){ 
	//	LOG("Window can't be created\nPavon Engine was closed\n");
	//	exit(-1);
	//}
	//win->window = new_window;
	
	if (pe_renderer_type == PEWMOPENGLES2) {
		//glfwMakeContextCurrent(win->window);
	}   

    //glfwSetWindowUserPointer(win->window,win);

    camera_heigth_screen = INIT_WINDOW_SIZE_Y;
    camera_width_screen = INIT_WINDOW_SIZE_X;
		window_update_viewport(INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y);
    
    win->initialized = true;
}

void window_manager_init_window(EngineWindow* window){
	if(window->init != NULL)
		window->init();
	window->initialized = true;
}

void window_manager_draw_windows(){
	
	for(u8 i = 0; i<engine_windows.count ; i++ ){
		EngineWindow* window = array_get(&engine_windows,i);
		if (pe_renderer_type == PEWMOPENGLES2) {
#ifdef LINUX
	    	glfwMakeContextCurrent(window->window);
#endif
		}
		if(!window->initialized)
			   continue;
#ifdef LINUX
	if(glfwWindowShouldClose(window->window)){
		window->finish();	
		//LOG("Window close\n");
		continue;
	}
#endif		
	window->draw();
	
	if(pe_renderer_type == PEWMOPENGLES2)	{

#ifdef LINUX
        glfwSwapBuffers(window->window);

#endif
	}

	}
}
