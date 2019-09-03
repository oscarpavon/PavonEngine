#include "editor.h"

int main(){

    init_engine_memory();

    windows_manager_init();    

    engine_init();

    engine_init_render();    
    editor_init();

    while(!engine_initialized){}//wait for initilization

    
    while (!glfwWindowShouldClose(window_editor_main.window))
    {
        window_update_envents();
        
        window_update_windows_input();    
        
        editor_update();    
        usleep(2*1000);    
        
    }
    editor_finish();
    
    
}
