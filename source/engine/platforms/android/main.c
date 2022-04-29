#include <engine/engine.h>

#include <engine/platforms/android/android.h>

#include "modeling.h"

Model* sphere;

void ainit(){

  camera_init(&main_camera); 
  init_vec3(-7,0,0, main_camera.position);
  camera_update(&main_camera);
  
	add_element_with_model_path("/sdcard/Download/sphere.glb");
	
	pe_change_background_color(0,1,0,1);

	LOG("################### GAMEEEEE INTTT   ###########");

  sphere = array_get_last(actual_model_array);
   
	init_modeling();
}

void draw(){

	draw_vertices();
}

void aloop(){


}


void ainput(){
	

	if(key_released(&input.K)){
		LOG("$$$$$$$$$$    WORK");
	}

  
	if(key_released(&input.A)){
		LOG("A");
		pe_element_set_position(selected_element,VEC3(0,0,-2));
	}
	
  if(key_released(&input.D)){
		LOG("D");
		pe_element_set_position(selected_element,VEC3(0,0,2));
	}

	if(key_released(&input.W)){
		LOG("W");
		pe_element_set_position(selected_element,VEC3(0,2,0));
	}

	if(key_released(&input.S)){
		LOG("S");
		pe_element_set_position(selected_element,VEC3(0,-2,0));
	}

}

void android_main(struct android_app* state) {


  PGame chess;
  ZERO(chess);

  chess.name = "Chess";
  chess.loop = &aloop;
  chess.init = &ainit;
	chess.draw = &draw; 
	chess.input = &ainput;
	chess.app = state;
	game = &chess;	//need for egl context creation

  //state->userData = &engine;
	game->app->onAppCmd = &pe_android_handle_cmd;
  game->app->onInputEvent = &pe_android_input_handle;

	LOG("################### where the fuck ###########");

	//pe_log("hello world, from android read file");
  pe_game_create(&chess);

}



