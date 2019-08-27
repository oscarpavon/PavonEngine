#include "windows_manager.h"
#include <stdio.h>
#include "text.h"
#include "editor.h"
#include "../Engine/game.h"

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


void window_update_windows_input(){

    if(editor_sub_mode == EDITOR_SUB_MODE_NULL){
        if(key__released(&input.A,GLFW_MOD_SHIFT)){
            if(editor_window_content_open){
                window_set_focus(&window_content_browser);
            }
            editor_window_content_open = true;
            
        }
    }

    if(window_editor_main.focus)
        editor_window_level_editor_input_update();

    if(window_content_browser.focus)
        editor_window_content_browser_input_update();

  
}