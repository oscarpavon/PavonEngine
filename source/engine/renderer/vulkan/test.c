#include "vulkan.h"
#include <engine/engine.h>
#include "draw.h"

int main() {

    pe_init();

    EngineWindow new_window;
    ZERO(new_window);
    array_add(&engine_windows, &new_window);
    EngineWindow *main_window = array_pop(&engine_windows);
    //main_window->init = &pe_vk_init;
    
    window_create(main_window, NULL, "PavonEngine");

    current_window = main_window;

    PEThreadCommand thread_commad;
    thread_commad.command = &window_manager_init_window;
    thread_commad.data = main_window;
    thread_commad.done = false;
    thread_commad.type = POINTER;
    array_add(&render_thread_commads, &thread_commad);

    pe_render_thread_init(); 

    while (1)
    {
        window_update_envents();

        
    }
    LOGW("test warning");
    

    return 0;
}
