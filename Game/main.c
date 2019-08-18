#include <stdlib.h>
#include "../Editor/windows_manager.h"
#include "../engine.h"
#include "../game.h"
#include "../Engine/level.h"
#include "../Engine/components/components.h"

int main(){
    init_engine_memory();
    EditorWindow main_window;
    create_window(&main_window);  
      
    draw_loading_screen();
    glfwSwapBuffers(main_window.window);
    
    init_engine();  
    init_game_engine();
    
    int level_result = load_level_to_elements_array("../levels/test.lvl", actual_elements_array);
    if(level_result != 0){
        debug_break();
        return -1;   
    }
        
    load_gui("test");

    for(int i = 0; i< texts.count ; i++){
       load_and_initialize_simple_model(get_from_array(&texts,i));
    }    

    for(int i = 0; i< textures_paths.count ; i++){
       load_simple_image(get_from_array(&textures_paths,i));
    } 


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