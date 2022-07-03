#include "android.h"
#include <android/input.h>
#include <engine/windows_manager.h>
#include <engine/game.h>
#include <engine/log.h>
#include <engine/input.h>

int32_t pe_android_input_handle(struct android_app *app, AInputEvent *event) {
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
    //   LOG("KEY PRESSED");
    int32_t action = AKeyEvent_getAction(event);
    int32_t key_code = AKeyEvent_getKeyCode(event);

    Key *current_key = NULL;

    switch (key_code) {
    case AKEYCODE_K:
      current_key = &input.K;
      break;
    case AKEYCODE_Q:
      current_key = &input.Q;
      break;
    case AKEYCODE_A:
      current_key = &input.A;
      break;
    case AKEYCODE_S:
      current_key = &input.S;
      break;
    case AKEYCODE_D:
      current_key = &input.D;
      break;
    case AKEYCODE_R:
      current_key = &input.R;
      break;
    case AKEYCODE_W:
      current_key = &input.R;
      break;
    case AKEYCODE_Y:
      current_key = &input.Y;
      break;
    case AKEYCODE_V:
      current_key = &input.V;
      break;
    case AKEYCODE_TAB:
      current_key = &input.TAB;
      break;
    case AKEYCODE_E:
      current_key = &input.E;
      break;
    case AKEYCODE_J:
      current_key = &input.J;
      break;
    default:
      break;
    }

    if (current_key) {
      if (action == AKEY_EVENT_ACTION_DOWN) {
        current_key->pressed = true;
        current_key->Released = false;
      }
      if (action == AKEY_EVENT_ACTION_UP) {
        current_key->pressed = false;
        current_key->Released = true;
      }
    }
    return 1;
  }

  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_DRAG) {

    //		engine->touchX = AMotionEvent_getX(event, 0);
    //		engine->touchY = AMotionEvent_getY(event, 0);
    //		ALOGI("x %d\ty %d\n",engine->touchX,engine->touchY);
    return 1;
  }

  float touch_x = AMotionEvent_getX(event, 0);
  float touch_y = AMotionEvent_getY(event, 0);

  //    LOG("##### Input touch X: %f Y: %f", touch_x, touch_y);
  touch_position_x = touch_x;
  touch_position_y = touch_y;

  //  LOG("################ Input handle ####################");
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
    pe_is_window_terminate = true; 
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
