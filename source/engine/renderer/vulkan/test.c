#include "vulkan.h"
#include <engine/engine.h>

void lala(){

}
int main() {
  pe_wm_renderer_type = PEWMVULKAN;

  pe_init();
	
  EngineWindow new_window;
  array_add(&engine_windows, &new_window);
  EngineWindow *main_window = array_pop(&engine_windows);
  window_create(main_window, NULL, "Vulkan");
		
  current_window = main_window;
  
  int extention_count = 0;
  char **extension;
  extension = glfwGetRequiredInstanceExtensions(&extention_count);
  LOG("Extension required count: %i\n", extention_count);

  //LOG("%s\n", extension[0]);
  //LOG("%s\n", extension[1]);

// pe_vk_init();
// pe_vk_end();

  main_window->init = &lala;
  
  
   PEThreadCommand thread_commad;
    thread_commad.command = &window_manager_init_window;
    thread_commad.data = main_window;
    thread_commad.done = false;
    thread_commad.type = POINTER;
    array_add(&render_thread_commads, &thread_commad);

    engine_init_render();


   while (1)
    {
        window_update_envents();
        
        				  
		}
	LOGW("test warning");

  return 0;
}
