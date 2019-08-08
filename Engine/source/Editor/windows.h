#ifndef WINDOWS_H
#define WINDOWS_H

#define GLFW_INCLUDE_ES2
#define GLFW_INCLUDE_GLEXT


#include <GLFW/glfw3.h>

#include "../camera.h"

typedef struct window{
       GLFWwindow* window; 
}Window;

void window_resize_callback(GLFWwindow* window, int width, int height);

void create_window(Window *win);
void update_envents();

float actual_window_width;
float actual_window_height;

#endif // !WINDOWS_H