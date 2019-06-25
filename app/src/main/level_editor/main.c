#include <stdlib.h>
#include "windows.h"
#include "../engine.h"
#include "input.h"


int main(){

    Window main_window;
    create_window(&main_window);
     
    Engine engine;
    glfwSetWindowUserPointer(main_window.window, &engine);
    glfwSetKeyCallback(main_window.window, key_callback);
	glfwSetCursorPosCallback(main_window.window, mouse_callback);
	glfwSetMouseButtonCallback(main_window.window, mouse_button_callback);

    init_engine();
    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();

        update_editor();
        update_input(&engine);

        glfwSwapBuffers(main_window.window);
    }
    glfwTerminate();
}