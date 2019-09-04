#ifndef WINDOWS_H
#define WINDOWS_H

#define GLFW_INCLUDE_ES2
#define GLFW_INCLUDE_GLEXT

#include <GLFW/glfw3.h>

#include "../../Engine/camera.h"
#include "tabs.h"

typedef struct EditorWindow{
	Array tabs;
    char name[20];
    bool focus;
    bool initialized; 
    GLFWwindow* window;
	void(*draw)(void);
	void(*init)(void);
	void(*finish)(void);
	EditorTab* tab_current;
	u8 tab_current_id;
}EditorWindow;

void window_resize_callback(GLFWwindow* window, int width, int height);
void window_focus_callback(GLFWwindow*,int);
void window_create(EditorWindow *win,EditorWindow* share_window,const char* name);

void window_initialize_windows();

inline static void window_update_envents(){
       glfwPollEvents();
}

void window_set_focus(EditorWindow* window);

void windows_manager_init();

void window_update_windows_input();

float actual_window_width;
float actual_window_height;

bool editor_window_content_open;

EditorWindow window_editor_main;

EditorWindow* current_window;

#endif // !WINDOWS_H
