#include <stdlib.h>
#include "windows.h"
#include "../engine.h"
#include "input.h"

#include "editor.h"

int main(){

   
    Window main_window;
    create_window(&main_window);    
      
    glfwSetKeyCallback(main_window.window, key_callback);
	glfwSetCursorPosCallback(main_window.window, mouse_callback);
	glfwSetMouseButtonCallback(main_window.window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(main_window.window, window_resize_callback);
    glfwSetCharCallback(main_window.window, character_callback);

    draw_loading_screen();
    glfwSwapBuffers(main_window.window);
    
    init_engine();
    init_editor();

    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();

        struct timespec time1, time2;
        int temp;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

        update_editor();
        update_input();
   
        glfwSwapBuffers(main_window.window);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

        struct timespec result = diff(time1,time2);
        frame_time = result.tv_nsec / 1000000;        
    }
    clean_editor();
    
    glfwTerminate();
}