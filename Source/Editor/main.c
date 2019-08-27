#include <stdlib.h>
#include "windows_manager.h"
#include "../Engine/engine.h"
#include "input.h"

#include "editor.h"

void editor_render_thread_init(){
    window_create(&window_editor_main, NULL, "Engine"); 

    glfwSetKeyCallback(window_editor_main.window, key_callback);
	glfwSetCursorPosCallback(window_editor_main.window, mouse_callback);
	glfwSetMouseButtonCallback(window_editor_main.window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window_editor_main.window, window_resize_callback);
    glfwSetCharCallback(window_editor_main.window, character_callback);
    glfwSetWindowFocusCallback(window_editor_main.window,window_focus_callback);

    compiles_standard_shaders();
    draw_loading_screen();
    glfwSwapBuffers(window_editor_main.window);    
}

int main(){

    init_engine_memory();

    windows_manager_init();    

    engine_init();

    ExecuteCommand command;
    command.command = &editor_render_thread_init;
    array_add(&array_render_thread_init_commmands,&command);

    editor_init();

    engine_init_render();    

    while(!engine_initialized){}//wait for initilization

    
    while (!glfwWindowShouldClose(window_editor_main.window))
    {
        window_update_envents();
        
        window_update_windows_input();    
        
        editor_update();        
        
    }
    editor_finish();
    
    glfwTerminate();
}