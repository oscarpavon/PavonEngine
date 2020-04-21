#include "render_thread.h"
#include <engine/engine.h>
#include <engine/types.h>
#include <engine/threads.h>


void render_thread_init(){
  pe_thread_control(&render_thread_commads);

  render_thread_definition.init();

  engine_initialized = true;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  camera_init(&main_camera);
  init_gui();
  text_renderer_init();

  camera_update(&main_camera);
}

void engine_render_thread() {

//*********  Timing ******
  float render_frame_time = 0;
  float disired_frame_time = 0.016f;

  u8 frames = 0;
  float frame_second = 0;
//************************

  render_thread_init();
  
	while (engine_running) {


    render_frame_time += time_delta;

    time_start();

		pe_thread_control(&render_thread_commads);

		render_thread_definition.draw();
		
		//********* Timing **********
    time_end();
    frame_second += time_elapsed_time;
    if (frame_second >= 1000) {
      FPS = frames * (1000.f / frame_second);
      frames = 0;
      frame_second = 0;
    } else
      frames++;
		//********* End timing ********
  
	}
	//end while
	render_thread_definition.end();
}

/*Init the render thread*/
void engine_init_render(){
    thread_new_detached(engine_render_thread,NULL,"Render");    
}
