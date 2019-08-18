#include <stdlib.h>
#include "windows_manager.h"
#include "../engine.h"
#include "input.h"

#include "editor.h"

int main(){

    init_engine_memory();

    windows_manager_init();    

    window_create(&window_editor_main, NULL, "Engine"); 

    glfwSetKeyCallback(window_editor_main.window, key_callback);
	glfwSetCursorPosCallback(window_editor_main.window, mouse_callback);
	glfwSetMouseButtonCallback(window_editor_main.window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_editor_main.window, window_resize_callback);
    glfwSetCharCallback(window_editor_main.window, character_callback);
    glfwSetWindowFocusCallback(window_editor_main.window,window_focus_callback);

    draw_loading_screen();
    glfwSwapBuffers(window_editor_main.window);
    
    init_engine();
    init_editor();

    while (!glfwWindowShouldClose(window_editor_main.window))
    {
        update_envents();

        struct timespec time1, time2;
        int temp;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
        
        update_input();
        draw_editor_viewport();   

        glfwSwapBuffers(window_editor_main.window);       
                
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

        struct timespec result = diff(time1,time2);
        frame_time = result.tv_nsec / 1000000;        
    }
    clean_editor();
    
    glfwTerminate();
}