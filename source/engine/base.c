#include "base.h"

#include "engine.h"

void pe_end(){
    engine_running = false;   
    clear_engine_memory();
}

void pe_init(){
  init_engine_memory();//VERY IMPORTANT

  pe_init_arrays();

	engine_running = true;
 
  pe_change_background_color(1,0,0,1);

#ifdef LINUX
  pe_th_main_id = pthread_self();
  pe_vk_initialized = false;
  //pe_audio_init();
  //pe_phy_init();
  pe_input_init();
  windows_manager_init();
#endif
}
