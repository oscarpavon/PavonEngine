#include <stdlib.h>
#include "windows.h"
#include "../engine.h"

int main(){
    Window main_window;
    create_window(&main_window);

    init_engine();
    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();


    }
    glfwTerminate();
}