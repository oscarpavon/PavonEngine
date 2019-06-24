#ifndef WINDOWS_H
#define WINDOWS_H

#define GLFW_INCLUDE_ES2
#define GLFW_INCLUDE_GLEXT


#include <GLFW/glfw3.h>



typedef struct window{
       GLFWwindow* window; 
}Window;



void create_window(Window *win);
void update_envents();

#endif // !WINDOWS_H