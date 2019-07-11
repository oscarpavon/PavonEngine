#include "windows.h"
#include <stdio.h>
#include "text.h"
#include "editor.h"
#include "../game.h"

void create_window(Window *win){
  
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    glfwInit();
    
    win->window = glfwCreateWindow(800,600,"Engine", NULL , NULL);
    glfwMakeContextCurrent(win->window);
    //glfwSetWindowMonitor(win->window, glfwGetPrimaryMonitor(), 0 , 0 , 800, 600, 0); 
  
    
    glViewport(0,0,800,600);
    camera_heigth_screen = 600;
    camera_width_screen = 800;
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
