#include <stdlib.h>
#include "../level_editor/windows.h"
#include "../engine.h"

int main(){

    Window main_window;
    create_window(&main_window);  
      
    draw_loading_screen();
    glfwSwapBuffers(main_window.window);
    
    init_engine();  
    init_game_engine();
    

    Array load_elements;
    init_array(&load_elements,sizeof(Element));
    load_level_to_elements_array("../levels/player.lvl", &load_elements);   
    
    add_loaded_elements(&load_elements, actual_model_array, actual_elements_array);
    clean_array(&load_elements);

    init_game();

    while (!glfwWindowShouldClose(main_window.window))
    {
        update_envents();
   
        update_game();
        
        engine_loop();      
    
        glfwSwapBuffers(main_window.window);
    }   
    
    glfwTerminate();
}