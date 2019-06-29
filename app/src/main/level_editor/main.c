#include <stdlib.h>
#include "windows.h"
#include "../engine.h"
#include "input.h"

#include "editor.h"

int main(){

   
    Window main_window;
    create_window(&main_window);    
   
    create_input_thread();

   
    glfwSetKeyCallback(main_window.window, key_callback);
	glfwSetCursorPosCallback(main_window.window, mouse_callback);
	glfwSetMouseButtonCallback(main_window.window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(main_window.window, window_resize_callback);

    draw_loading_screen();
    glfwSwapBuffers(main_window.window);
    
    init_engine();
    init_editor();

    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();

        update_editor();
        update_input();
   
        glfwSwapBuffers(main_window.window);
    }
    clean_editor();
    
    glfwTerminate();
}