#include "render_thread.h"
#include <engine/engine.h>
#include <engine/types.h>
#include <engine/threads.h>
#include <engine/renderer/renderer.h>

void engine_draw_elements(Array *elements) {
  for (size_t i = 0; i < elements->count; i++) {
    Model *draw_model = array_get_pointer(elements, i);
    if (!draw_model)
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
  pe_thread_control(&render_thread_commads);//for execute program_ini() from application

  pe_shader_compile_std();

#ifdef LINUX
  glEnable(GL_MULTISAMPLE);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);


  if(render_thread_definition.init != NULL)
    render_thread_definition.init();

  engine_initialized = true;

  pe_gui_init();
#ifdef LINUX
  text_renderer_init();
#endif
   

}

void engine_render_draw() {

    render_thread_init();
    
    //*********  Timing ******
    float render_frame_time = 0;
    float disired_frame_time = 0.016f;
   
    u8 frames = 0;
    float frame_second = 0;
    //************************
     
  
    while (engine_running) {

        render_frame_time += time_delta;

        time_start();

		pe_thread_control(&render_thread_commads);

        if(render_thread_definition.draw != NULL)
		    render_thread_definition.draw();
		
		//********* Timing **********
        time_end();
       
        frame_second += time_elapsed_time;
        
        if (frame_second >= 1000) {
            FPS = frames * (1000.f / frame_second);
            frames = 0;
            frame_second = 0;
        }else{
            frames++;
        }
    	//********* End timing ********
  
	}
	//end while
	render_thread_definition.end();
}

/*Start render thread and call engine_render_draw()*/
void pe_render_thread_start_and_draw(){
	thread_new_detached(engine_render_draw,NULL,"Render",&pe_th_render_id);    
}

void pe_frame_static_fill(ComponentDefinition* definition){
  StaticMeshComponent* mesh_comp = definition->data;
  if(mesh_comp->models_p.initialized == false)
    return;
  Model* model = (Model*)array_get_pointer(&mesh_comp->models_p,0);
  //model->shader = mesh_comp->material.shader;
  model->material = mesh_comp->material;

  array_add_pointer(&frame_draw_static_elements,model) ;

}

void pe_frame_draw(){

  glClearColor(1,0,0,1);
  render_clear_buffer(RENDER_COLOR_BUFFER | RENDER_DEPTH_BUFFER);

  for_each_element_components(&update_per_frame_component);

  //test_elements_occlusion();
  //check_meshes_distance();
  for_each_element_components(&pe_frame_static_fill);

  engine_draw_elements(&frame_draw_static_elements);

  pe_frame_clean();

}
