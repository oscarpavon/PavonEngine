#include <stdlib.h>
#include "windows_manager.h"
#include "../Engine/engine.h"
#include "input.h"

#include "editor.h"

int main(){

    init_engine_memory();

    windows_manager_init();    

    engine_init();

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