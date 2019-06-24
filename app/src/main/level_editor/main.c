#include <stdlib.h>
#include "windows.h"

int main(){
    Window main_window;
    create_window(&main_window);
    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();
    }
    glfwTerminate();
}