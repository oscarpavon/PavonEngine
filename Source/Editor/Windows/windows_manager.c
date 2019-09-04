#include "windows_manager.h"
#include <stdio.h>
#include "../text.h"
#include "../editor.h"
#include "../../Engine/game.h"

#include "content_browser.h"
#include "tabs.h"

#define INIT_WINDOW_SIZE_X 1280
#define INIT_WINDOW_SIZE_Y 720
void window_manager_init_window(EditorWindow* window){
	window->init();
	window->initialized = true;
}
void windows_manager_init(){
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwInit();
}

void window_create(EditorWindow *win, EditorWindow* share_window, const char* name){

    if(win->initialized)
        return;

    current_window = win;
    
    GLFWwindow* share_glfw_window = NULL;
    if(share_window)
     share_glfw_window = share_window->window;

    win->window = glfwCreateWindow(INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y,name, NULL ,share_glfw_window );
    glfwMakeContextCurrent(win->window);
    
    glfwSetWindowUserPointer(win->window,win);

    glViewport(0,0,INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y);
    camera_heigth_screen = INIT_WINDOW_SIZE_Y;
    camera_width_screen = INIT_WINDOW_SIZE_X;
    
    win->initialized = true;
}

void window_resize_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    camera_heigth_screen = height;
    camera_width_screen = width;

    update_viewport_size();
}

void window_focus_callback(GLFWwindow* window,int is_focus){
    EditorWindow* editor_window = glfwGetWindowUserPointer(window);
    if(is_focus == GLFW_TRUE){
        editor_window->focus = true;
    }
    if(is_focus == GLFW_FALSE){
       editor_window->focus = false;
    }
}

void window_set_focus(EditorWindow* window){
    current_window->focus = false;
    glfwShowWindow(window->window);
    glfwFocusWindow(window->window);
    memset(&input,0,sizeof(Input));
    glfwMakeContextCurrent(window->window);
    window->focus = true;
    current_window = window;
    LOG("Focus windows change\n");
}

void window_manager_draw_windows(){
	//Draw tab bar 	& draw current tabb 
	for(u8 i = 0; i<editor_windows.count ; i++ ){
		EditorWindow* window = array_get(&editor_windows,i);
		glfwMakeContextCurrent(window->window);
		if(!window->initialized)
			   continue;
		
	if(glfwWindowShouldClose(window->window)){
		glfwDestroyWindow(window_content_browser->window);
		window->finish();	
		LOG("Window close\n");
		continue;
	}
		if(window->tab_current && window->tab_current->draw)		
			window->tab_current->draw();
		if(window->tabs.count > 0){
			tabs_draw_tabs_bar(window_editor_main);	
		}
		else
			window->draw();
		
		for(u8 j = 0; j < window->tabs.count ; j++){
			EditorTab* tab = array_get(&window->tabs,j);	

		}

		
    	glfwSwapBuffers(window->window);
	}
}

void window_update_windows_input(){
	
    if(editor_sub_mode == EDITOR_SUB_MODE_NULL){
        if(key__released(&input.A,GLFW_MOD_SHIFT)){
		   if(!window_content_browser->initialized)
				window_manager_init_window(window_content_browser);			
        	else
				window_set_focus(window_content_browser);
		
		}
		if(key__released(&input.N,GLFW_MOD_SHIFT)){
			tabs_new(current_window,"New Tab");
			LOG("New tab added to Window: %s\n",current_window->name);	
		}
    }
	//Draw tab bar 	& draw current tabb 
	for(u8 i = 0; i<editor_windows.count ; i++ ){
		EditorWindow* window = array_get(&editor_windows,i);
		if(!window->initialized)
			   continue;
		for(u8 j = 0; j < window->tabs.count ; j++){
			EditorTab* tab = array_get(&window->tabs,j);
		}
		if(window->focus){
			if(window->input)
				window->input();
		}
	}
}

void window_initialize_windows(){
	for(u8 i = 0; i<editor_windows.count ; i++ ){
		EditorWindow* window = array_get(&editor_windows,i);
		if(window->initialized)
			   continue;
		window->init();	
	}
}


void window_manager_create_editor_windows_data(){

	EditorWindow content_browser_window;
	memset(&content_browser_window,0,sizeof(EditorWindow));
	content_browser_window.init = editor_window_content_init;
	content_browser_window.draw = editor_window_content_browser_draw;
	content_browser_window.input = editor_window_content_browser_input_update;
	content_browser_window.finish = editor_window_content_browser_close_window;
	array_add(&editor_windows,&content_browser_window);
	window_content_browser = array_pop(&editor_windows);	

}
