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

	FILE* new_file = fopen("/storage/emulated/0/Download/test.txt","a");
	fputs(text,new_file);
	fclose(new_file);
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



/**
 * Process the next input event.
 */
int32_t handle_input(struct android_app* app, AInputEvent* event) {
	struct engine_t* engine = (struct engine_t*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->touchX = AMotionEvent_getX(event, 0);
		engine->touchY = AMotionEvent_getY(event, 0);
		ALOGI("x %d\ty %d\n",engine->touchX,engine->touchY);
		return 1;
	}
	return 0;
}


void ainit(){

  camera_init(&main_camera); 
  init_vec3(-7,3.5,3.4, main_camera.position);
  camera_update(&main_camera);
  
	//add_element_with_model_path("/sdcard/Download/model.glb");
	pe_change_background_color();

}


void aloop(){


}

void ainput(){


}

void android_main(struct android_app* state) {

  //state->userData = &engine;
  state->onAppCmd = pe_android_handle_cmd;
  //state->onInputEvent = handle_input;

  PGame chess;
  ZERO(chess);

  chess.name = "Chess";
  chess.loop = &aloop;
  chess.init = &ainit;
  chess.input = &ainput;
	chess.app = state;
	game = &chess;	//need for egl context creation


	LOG("################### where the fuck ###########");


  pe_game_create(&chess);

}



