#ifndef WINDOWS_H
#define WINDOWS_H

#define GLFW_INCLUDE_ES2
#define GLFW_INCLUDE_GLEXT


#include <GLFW/glfw3.h>

#include "../camera.h"

typedef struct EditorWindow{
       GLFWwindow* window;
       bool focus;
       bool initialized; 
}EditorWindow;

EditorWindow* current_window;

void window_resize_callback(GLFWwindow* window, int width, int height);
void window_focus_callback(GLFWwindow*,int);
void window_create(EditorWindow *win,EditorWindow* share_window,const char* name);
void update_envents();

void windows_manager_init();

void windows_update();

float actual_window_width;
float actual_window_height;

bool editor_window_content_open;

EditorWindow window_editor_main;

#endif // !WINDOWS_H