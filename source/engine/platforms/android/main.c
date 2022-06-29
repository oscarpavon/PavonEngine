#include <engine/engine.h>

#include <engine/platforms/android/android.h>

#include "modeling.h"

#include <engine/../../demos/chess/chess.h>

#include <editor/editor.h>

Model* sphere;

void ainit() {

  camera_init(&main_camera);
  init_vec3(-30, 0, 0, main_camera.position);
  camera_update(&main_camera);

  add_element_with_model_path("/data/data/com.termux/files/home/PavonEngine/demos/chess/peon.glb");

  pe_change_background_color(1, 0, 0, 1);

  LOG("################### GAMEEEEE INTTT   ###########");

  sphere = array_get_last(actual_model_array);

  init_modeling();
  pe_element_set_position(selected_element, VEC3(0, 0, 0));
}

void draw(){

	draw_vertices();
}

void aloop(){


}


void ainput(){
	

	if(key_released(&input.K)){

		vertex_new(1,1,1)	;

	}

  
	if(key_released(&input.A)){
		vertex_new(0,0,0.001)	;
	}
	
  if(key_released(&input.D)){
		vertex_new(1,1,1)	;
	}

	if(key_released(&input.W)){
	
		vertex_new(0.01,0,0)	;
	}


}

void android_main(struct android_app* state) {


  PGame chess;
  ZERO(chess);

  chess.name = "Chess";
  chess.loop = &chess_loop;
  chess.init = &chess_init;
	chess.draw = &chess_draw; 
	chess.input = &chess_input;
	chess.app = state;
	game = &chess;	//need for egl context creation

  //state->userData = &engine;
	game->app->onAppCmd = &pe_android_handle_cmd;
  game->app->onInputEvent = &pe_android_input_handle;

	LOG("################### where the fuck ###########");

	//pe_log("hello world, from android read file");
  pe_game_create(&chess);
 


pe_renderer_type = PEWMOPENGLES2;


  //pe_editor_init();

 // pe_program_main_loop(&editor_main_loop, window_editor_main);


}
































//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
