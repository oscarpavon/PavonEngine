#include <jni.h>
#include <errno.h>
#include <math.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <engine/engine.h>
#include <engine/base.h>
#include <engine/array.h>

#include "/data/data/com.termux/files/home/newlib/show.h"

#include <android/asset_manager.h>
#include <stdio.h>

#include <engine/platforms/android/android.h>

void pe_log(const char* text){

	const char* path = "/storage/emulated/0/Download/test.txt";
	const char* path2 = "/sdcard/Download/cube.gltf";

	FILE* new_file = fopen(path,"a");
	fputs(text,new_file);
	fclose(new_file);

    FILE* file = fopen(path2,"r");
    if(file == NULL){
        LOG("error to load: %s\n", path2);
     //   return -1;
    }
	
}

struct engine_t {
	struct android_app* app;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;

	int32_t touchX;
	int32_t touchY;
};




void ainit(){

  camera_init(&main_camera); 
  init_vec3(-7,3.5,3.4, main_camera.position);
  camera_update(&main_camera);
  
	add_element_with_model_path("/sdcard/Download/sphere.glb");
	
	pe_change_background_color(0,1,0,1);

	LOG("################### GAMEEEEE INTTT   ###########");
}


void aloop(){

	pe_change_background_color(0,0.3,0,1);
}


void ainput(){


}

void android_main(struct android_app* state) {

  //state->userData = &engine;
  state->onAppCmd = &pe_android_handle_cmd;
  state->onInputEvent = &pe_android_input_handle;

  PGame chess;
  ZERO(chess);

  chess.name = "Chess";
  chess.loop = &aloop;
  chess.init = &ainit;
  chess.input = &ainput;
	chess.app = state;
	game = &chess;	//need for egl context creation


	LOG("################### where the fuck ###########");

	pe_log("hello world, from android read file");
  pe_game_create(&chess);

}



