#include "android.h"
#include <android/input.h>
#include <engine/windows_manager.h>
#include <engine/game.h>
#include <engine/log.h>
#include <engine/input.h>

int32_t pe_android_input_handle(struct android_app *app, AInputEvent *event) {
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
    //		engine->touchX = AMotionEvent_getX(event, 0);
    //		engine->touchY = AMotionEvent_getY(event, 0);
    //		ALOGI("x %d\ty %d\n",engine->touchX,engine->touchY);
    LOG("KEY PRESSED");
    int32_t action = AKeyEvent_getAction(event);
    if (action == AKEY_EVENT_ACTION_DOWN) {

      int32_t key_code = AKeyEvent_getKeyCode(event);
      switch (key_code) {
      case AKEYCODE_K: {
        LOG("The 'K' key was pressed");
        input.K.Released = true;
      }
      case AKEYCODE_A: input.A.Released = true;
      case AKEYCODE_S: input.S.Released = true;
      case AKEYCODE_D: input.D.Released = true;
      case AKEYCODE_W: input.W.Released = true;
      }
    }
    return 1;
  }

  LOG("################ Input handle ####################");
  return 0;
}

void pe_android_handle_cmd(struct android_app *app, int32_t cmd) {
  LOG("##### Android CMD ######");
  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    break;
  case APP_CMD_INIT_WINDOW:
    //pe_wm_egl_init();
    //pe_wm_swap_buffers();

    //pe_game_create(game);
    LOG("############## android windows init  #############");
    pe_is_window_init = true;
    break;
  case APP_CMD_TERM_WINDOW:

    LOG("##### Android TERM WINDOW ######");
    pe_wm_egl_end();
    break;
  case APP_CMD_LOST_FOCUS:
    
    LOG("##### Android LOST FOCUS ######");
    pe_wm_swap_buffers();
    break;
  }
}

void pe_android_poll_envents(){
    struct android_app* state = game->app;
    int ident;
    int events;
    struct android_poll_source *source;

    while ((ident = ALooper_pollAll(0, NULL, &events, (void **)&source)) >= 0) {

      // Process this event.
      if (source != NULL) {
        source->process(state, source);
      }

      // Check if we are exiting.
      if (state->destroyRequested != 0) {
				pe_wm_egl_end();	
        return;
      }
    }
}
