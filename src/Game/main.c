#include <stdlib.h>
#include "../level_editor/windows.h"
#include "../engine.h"

int main(){

    Window main_window;
    create_window(&main_window);  
      
    draw_loading_screen();
    glfwSwapBuffers(main_window.window);
    
    init_engine();  

    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();
   
   
        glfwSwapBuffers(main_window.window);
    }   
    
    glfwTerminate();
}