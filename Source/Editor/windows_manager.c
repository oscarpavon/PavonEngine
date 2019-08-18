#include "windows_manager.h"
#include <stdio.h>
#include "text.h"
#include "editor.h"
#include "../game.h"

#include "Windows/content_browser.h"

#define INIT_WINDOW_SIZE_X 1280
#define INIT_WINDOW_SIZE_Y 720

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
    
    glViewport(0,0,INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y);
    camera_heigth_screen = INIT_WINDOW_SIZE_Y;
    camera_width_screen = INIT_WINDOW_SIZE_X;
    
    win->initialized = true;
}

void update_envents(){
    glfwPollEvents();  
}

void window_resize_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    camera_heigth_screen = height;
    camera_width_screen = width;

    update_viewport_size();
}

void window_focus_callback(GLFWwindow* window,int is_focus){
    if(is_focus == GLFW_TRUE){
        current_window->focus = true;
    }
    if(is_focus == GLFW_FALSE){
       current_window->focus = false;
    }
}



void windows_update(){
    if(editor_sub_mode == EDITOR_SUB_MODE_NULL){
        if(key__released(&input.A,GLFW_MOD_SHIFT)){
            editor_window_content_open = true;
        }
    }


    if(editor_window_content_open){
        if(!window_content_browser.initialized){
            window_create(&window_content_browser,&window_editor_main,"Engine");
            editor_window_content_init();
        }
        editor_window_content_browser_draw();
        glfwMakeContextCurrent(window_editor_main.window);
    }
}