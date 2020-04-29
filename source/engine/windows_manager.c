#include "windows_manager.h"
#include <stdio.h>

#include "game.h"
#include "log.h"
#include "text_renderer.h"

#define INIT_WINDOW_SIZE_X 1280
#define INIT_WINDOW_SIZE_Y 720

void window_manager_error_callback(int error, const char* description)
{
	    fprintf(stderr, "PEglfw error: %s\n", description);
}

void pe_wm_glfw_init(){
  
	if (pe_wm_renderer_type == PEWMOPENGLES2) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  } else if (pe_wm_renderer_type == PEWMVULKAN) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }
	//MSAA
	glfwWindowHint(GLFW_SAMPLES,4);

  glfwSetErrorCallback(window_manager_error_callback);
  glfwInit();
}


void window_manager_init_window(EngineWindow* window){
	window->init();
	window->initialized = true;
}

void windows_manager_init(){
	pe_wm_glfw_init();		
	array_init(&engine_windows,sizeof(EngineWindow),40);
}

void window_create(EngineWindow *win, EngineWindow* share_window, const char* name){

    if(win->initialized)
        return;

    current_window = win;
    
    GLFWwindow* share_glfw_window = NULL;
    if(share_window)
     share_glfw_window = share_window->window;

		GLFWwindow* new_window = glfwCreateWindow( INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y,name, NULL ,share_glfw_window );
		if(!new_window){ 
			LOG("Window can't be created\nPavon Engine was closed\n");
			exit(-1);
		}
		win->window = new_window;
		glfwMakeContextCurrent(win->window);
    
    glfwSetWindowUserPointer(win->window,win);

    camera_heigth_screen = INIT_WINDOW_SIZE_Y;
    camera_width_screen = INIT_WINDOW_SIZE_X;
		window_update_viewport(INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y);
    
    win->initialized = true;
}

void window_update_viewport(int width, int height){
		glViewport(0,0,width,height);
    text_renderer_update_pixel_size();
		camera_update_aspect_ratio(&current_window->camera);
}

void window_resize_callback(GLFWwindow* window, int width, int height){
    camera_heigth_screen = height;
    camera_width_screen = width;
		window_set_focus(current_window); 
		window_update_viewport(width,height);
}

void window_focus_callback(GLFWwindow* window,int is_focus){
    EngineWindow* editor_window = glfwGetWindowUserPointer(window);
    if(is_focus == GLFW_TRUE){
        editor_window->focus = true;
    }
    if(is_focus == GLFW_FALSE){
       //editor_window->focus = false;
    }
}

void window_set_focus(EngineWindow* window){
    current_window->focus = false;
    glfwShowWindow(window->window);
    glfwFocusWindow(window->window);
    //memset(&input,0,sizeof(Input));
    glfwMakeContextCurrent(window->window);
    window->focus = true;
    current_window = window;
    //LOG("Focus windows change\n");
}

void window_manager_draw_windows(){
	
	for(u8 i = 0; i<engine_windows.count ; i++ ){
		EngineWindow* window = array_get(&engine_windows,i);
		glfwMakeContextCurrent(window->window);
		if(!window->initialized)
			   continue;
		
	if(glfwWindowShouldClose(window->window)){
		window->finish();	
		LOG("Window close\n");
		continue;
	}
		
		window->draw();
		
    glfwSwapBuffers(window->window);

	}
}

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

void window_initialize_windows(){
	for(u8 i = 0; i<engine_windows.count ; i++ ){
		EngineWindow* window = array_get(&engine_windows,i);
		if(window->initialized)
			   continue;
		window->init();	
	}
}


