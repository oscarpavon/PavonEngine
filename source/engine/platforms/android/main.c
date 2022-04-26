#include <jni.h>
#include <errno.h>
#include <math.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define APP_NAME "PavonEngine"
#define ALOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))
#define ALOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__))
#define ALOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))

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

}


void aloop(){


}

void ainput(){


}

void create_game(struct android_app* state){

  PGame chess;
  ZERO(chess);

  chess.name = "Chess";
  chess.loop = &aloop;
  chess.init = &ainit;
  chess.input = &ainput;
	chess.app = state;
	
  pe_game_create(&chess);
}

void android_main(struct android_app* state) {

    PGame n;
    ZERO(n);
    game = &n;

	//create_game(state);


	game->app = state;	
  //state->userData = &engine;
  state->onAppCmd = pe_android_handle_cmd;
  //state->onInputEvent = handle_input;

  while (1) {
		pe_android_poll_envents();		
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the current frame
    pe_change_background_color();
		pe_wm_swap_buffers();
    //draw_frame(&engine);
	}
}



