#include "windows_manager.h"
#include <engine/log.h>

#ifdef ANDROID
#include <engine/platforms/android/android.h>
#include <EGL/egl.h>
EGLDisplay display;
EGLSurface surface;
EGLContext context;
#endif

#include <engine/game.h>

#include <GLES/gl.h>


/**
 * Tear down the EGL context currently associated with the display.
 */
void pe_wm_egl_end() {
	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT) {
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE) {
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}

void pe_wm_swap_buffers() {
#ifdef ANDROID
  eglSwapBuffers(display, surface);
#endif

#ifdef LINUX
  glfwSwapBuffers(window->window);

#endif
}
void pe_wm_egl_context_make_current(){
	eglMakeCurrent(display, surface, surface, context);
}

void pe_wm_egl_init(){

	// Setup OpenGL ES 2
	// http://stackoverflow.com/questions/11478957/how-do-i-create-an-opengl-es-2-context-in-a-native-activity

	const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_DEPTH_SIZE,8,
			EGL_NONE
	};

	EGLint attribList[] =
	{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(game->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, game->app->window, NULL);

	context = eglCreateContext(display, config, NULL, attribList);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
	//	ALOGW("Unable to eglMakeCurrent");
		return -1;
	}

	// Grab the width and height of the surface
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	
	//engine->display = display;
	//engine->context = context;
	//engine->surface = surface;
	//engine->width = w;
	//engine->height = h;

	// Initialize GL state.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

  camera_heigth_screen = h;
  camera_width_screen = w;
  window_update_viewport(w, h);
	return 0;

}

void pe_wm_events_update() {
#ifdef LINUX
  glfwPollEvents();
#endif
#ifdef ANDROID
  pe_android_poll_envents();
#endif
}

void pe_wm_input_update() {

  // Draw tab bar 	& draw current tabb
  for (u8 i = 0; i < engine_windows.count; i++) {
    EngineWindow *window = array_get(&engine_windows, i);
		if (!window->initialized){
      continue;
		}

#ifdef LINUX
    // The mouse need to stay in the window for window->input call
    if (window->focus) {
      if (window->input)
        window->input();
    }
#endif

#ifdef ANDROID

		if (window->input){

      window->input();
		}
#endif
  }
}

bool pe_wm_should_close(EngineWindow* window){
	if(pe_is_window_terminate)	
		return true;
	else
    return false;
}


void windows_manager_init(){

}

void pe_input_init(){


}

void window_update_viewport(int width, int height){
		glViewport(0,0,width,height);
#ifdef LINUX
    text_renderer_update_pixel_size();
#endif
		camera_update_aspect_ratio(&current_window->camera);
}

void pe_wm_create_window(EngineWindow* win){
	
  if (win == NULL) {
    LOG("ERROR: Window not found\n");
    return;
  }
  if (win->initialized)
    return;

  current_window = win;


	pe_wm_egl_init();	

  win->initialized = true;
}

void window_create(EngineWindow *win, EngineWindow *share_window,
                   const char *name) {
  if (win == NULL) {
    // LOG("ERROR: Window not found\n");
    return;
  }
  if (win->initialized)
    return;

  current_window = win;

  //// GLFWwindow* share_glfw_window = NULL;
  // if(share_window)
  // share_glfw_window = share_window->window;

  if (pe_renderer_type == PEWMVULKAN) {
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }

  // GLFWwindow* new_window = glfwCreateWindow(
  // INIT_WINDOW_SIZE_X,INIT_WINDOW_SIZE_Y,name, NULL ,share_glfw_window );
  // if(!new_window){
  //	LOG("Window can't be created\nPavon Engine was closed\n");
  //	exit(-1);
  // }
  // win->window = new_window;

  // glfwSetWindowUserPointer(win->window,win);

  if (pe_renderer_type == PEWMOPENGLES2) {
    // glfwMakeContextCurrent(win->window);
  }
	
  camera_heigth_screen = INIT_WINDOW_SIZE_Y;
  camera_width_screen = INIT_WINDOW_SIZE_X;
  window_update_viewport(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);

  win->initialized = true;
}

void pe_wm_window_init(EngineWindow* window){
	if(window->init != NULL)
		window->init();
	window->initialized = true;
}


void pe_wm_context_current(){

#ifdef LINUX
      glfwMakeContextCurrent(window->window);
#endif
#ifdef ANDROID
      pe_wm_egl_context_make_current();
#endif

}


void pe_wm_windows_draw() {

  for (u8 i = 0; i < engine_windows.count; i++) {
    EngineWindow *window = array_get(&engine_windows, i);
    if (pe_renderer_type == PEWMOPENGLES2) {
      pe_wm_context_current();
    }
    if (!window->initialized)
      continue;
    if (pe_wm_should_close(window)) {
      window->finish();
      LOG("Window close\n");
      continue;
    }

    window->draw();

    if (pe_renderer_type == PEWMOPENGLES2) {

      pe_wm_swap_buffers();
    }
  }
}
