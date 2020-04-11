#include "render_thread.h"
#include "engine.h"
#include "types.h"
#include "threads.h"

bool engine_user_render_thread_initialized_in_loop = false;

void engine_render_thread_init(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  camera_init(&main_camera);
  init_gui();
  text_renderer_init();
}

void engine_client_initialize_render_thread(){
    if(array_render_thread_init_commmands.count == 0){
        LOG("Critical, no render thread initialize commmand\n");
        debug_break();
    }

    for (u8 i = 0; i < array_render_thread_init_commmands.count; i++)
    {
        ExecuteCommand* exectute = array_get(&array_render_thread_init_commmands,i);
        exectute->command(exectute->parameter);
    }
		array_clean(&array_render_thread_init_commmands);

    engine_user_render_thread_init();
    engine_initialized = true;
}

void engine_render_thread() {

//*********  Timing ******
  float render_frame_time = 0;
  float disired_frame_time = 0.016f;

  u8 frames = 0;
  float frame_second = 0;
//************************

  while (engine_running) {
		
		pe_thread_control(&render_thread_commads);	

   //Initialize 
		if (!engine_user_render_thread_initialized_in_loop) {
      if (engine_client_render_thread_initialized) {
        engine_client_initialize_render_thread();
        engine_render_thread_init();
        camera_update(&main_camera);
        engine_user_render_thread_initialized_in_loop = true;
      }
    }

    render_frame_time += time_delta;

    time_start();

    int executed_commmand_count = 0;
    for (u8 i = 0; i < array_render_thread_commands.count; i++) {
      ExecuteCommand *command = array_get(&array_render_thread_commands, i);
      if (command->executed == false) {
        command->command(command->parameter);
        command->executed = true;
        executed_commmand_count++;
      }
    }
    if (executed_commmand_count == 0) {
      array_clean(&array_render_thread_commands);
    }

    if (engine_client_render_thread_initialized &&
        engine_user_render_thread_initialized_in_loop)
      engine_user_render_thread_draw();

    time_end();
    frame_second += time_elapsed_time;
    if (frame_second >= 1000) {
      FPS = frames * (1000.f / frame_second);
      frames = 0;
      frame_second = 0;
    } else
      frames++;
  }
	//end while
  engine_user_render_thread_finish();
}

/*Init the render thread*/
void engine_init_render(){
		array_init(&render_thread_commads,sizeof(PEThread_Command),100);
    thread_new_detached(engine_render_thread,NULL,"Render");    
}
