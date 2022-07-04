#include <engine/engine.h>

#include <engine/platforms/android/android.h>

#include "modeling.h"

#include <engine/../../demos/chess/chess.h>

#include <editor/editor.h>


void ainit() {

  camera_init(&main_camera);
  init_vec3(-30, 0, 0, main_camera.position);
  camera_update(&main_camera);

  pe_change_background_color(1, 0, 0, 1);

  init_modeling();
}

void draw(){

	if(key_released(&input.A)){
		vertex_new(0,0,0)	;
	}
	
  if(key_released(&input.D)){
		vertex_new(0.1,0.1,0.1)	;
	}
	
  if(key_released(&input.K)){

		vertex_new(1,1,1)	;

	}

  if(key_released(&input.TAB)){
    pe_modeling_select_next_vertex();
  }

	draw_vertices();
}

void aloop(){


}


void ainput(){
	


  

	if(key_released(&input.W)){
		vertex_new(0.01,0,0)	;
	}
	
  if(key_released(&input.Q)){
    exit(0);
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

  PGame modelling;
  ZERO(modelling);
  modelling.name = "Modelling";
  modelling.init = &ainit;
  modelling.input = &ainput;
  modelling.loop = &aloop;
  modelling.draw = &draw;
	modelling.app = state;
	game = &modelling;	//need for egl context creation

  PGame editor;
  ZERO(editor);
  editor.name = "Editor";
  editor.init = &pe_editor_init; 
  editor.input = &editor_window_level_editor_input_update; // handle editor modes
  editor.loop = &aloop;
  editor.draw = &pe_editor_draw;
  editor.app = state;
  game = &editor;

	game->app->onAppCmd = &pe_android_handle_cmd;
  game->app->onInputEvent = &pe_android_input_handle;

  pe_game_create(&editor);




	//pe_log("hello world, from android read file");

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
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
