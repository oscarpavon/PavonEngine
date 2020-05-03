#include "render_thread.h"
#include <engine/engine.h>
#include <engine/types.h>
#include <engine/threads.h>


void engine_draw_elements(Array *elements){
    for(size_t i = 0; i < elements->count ; i++) { 
        Model** model = array_get(elements,i);
        Model* draw_model = model[0];  
				if(!draw_model)
					continue;			
        draw_simgle_model(draw_model);
    }
    array_clean(elements);
}

void pe_render_skinned_elements(Array* elements){
  for (size_t i = 0; i < elements->count; i++) {
		SkinnedMeshComponent** skinp= array_get(elements,i); 
		SkinnedMeshComponent* skin = skinp[0];
		
    pe_render_skinned_model(skin);
  }
	
  array_clean(elements);
	array_clean(&frame_draw_skinned_elements);
}

void pe_frame_clean(){
    //clean frame
    array_clean(&models_for_test_occlusion);
    array_clean(&array_static_meshes_pointers);
    array_clean(&array_static_meshes_pointers_for_test_distance);
    array_clean(&array_skinned_mesh_pointers);
    array_clean(&array_skinned_mesh_for_distance_test);
    for_each_element_components(&clean_component_value);
    //end clean frame
}
void render_thread_init(){
  pe_thread_control(&render_thread_commads);

  pe_shader_compile_std();

	glEnable(GL_MULTISAMPLE);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  render_thread_definition.init();

  engine_initialized = true;

  camera_init(&main_camera);
  pe_gui_init();
  text_renderer_init();

  camera_update(&main_camera);
}

void engine_render_thread() {
	LOG("Render thread id= %ld\n",pthread_self());

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
	thread_new_detached(engine_render_thread,NULL,"Render",&pe_th_render_id);    
}

void pe_frame_draw(){

  glClearColor(1,0,0,1);
  render_clear_buffer(RENDER_COLOR_BUFFER | RENDER_DEPTH_BUFFER);

  for_each_element_components(&update_per_frame_component);

  test_elements_occlusion();
  check_meshes_distance();

  engine_draw_elements(&frame_draw_static_elements);

  pe_frame_clean();

}
