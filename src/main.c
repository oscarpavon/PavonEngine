

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <vec3.h>
#include <cglm.h>

#include "utils.h"


#include "audio.h"
#include "camera.h"

#include "engine.h"
#include "input.h"
#include "file_loader.h"
#include "gui.h"
#include "Engine/level.h"
#include "game.h"

static EGLint const attribute_list[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        //EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
};

static const EGLint GiveMeGLES2[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
};


EGLDisplay display;
EGLConfig config;
EGLContext context;
EGLSurface surface;
EGLint num_config;

EGLint w = 0;
EGLint h = 0;


void create_window(struct android_app * app){


    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);


    eglInitialize(display, NULL , NULL);


    eglChooseConfig(display, attribute_list,&config, 1  , &num_config);

    EGLint format;
    eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_ID, &format);



    ANativeWindow_setBuffersGeometry(app->window,ANativeWindow_getWidth(app->window), ANativeWindow_getHeight(app->window),format);


    surface = eglCreateWindowSurface(display, config, app->window, NULL);


    context = eglCreateContext(display, config, NULL, GiveMeGLES2);



    eglQuerySurface(display, surface, EGL_WIDTH, &w);

    eglQuerySurface(display, surface, EGL_HEIGHT, &h);



    eglMakeCurrent(display, surface, surface, context);

    LOG("---------> Windows size: %f %f",(float)w,(float)h);

    glViewport(0,0,w,h);
    camera_heigth_screen = h;
    camera_width_screen = w;

}


void handle_cmd(struct android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {

            create_window(pApp);


            pApp->userData = pApp;

        }
            break;




        case APP_CMD_TERM_WINDOW:
            should_close = true;
            break;
    }
}


void android_main(struct android_app* main_app){
    app = main_app;

    app->onAppCmd = handle_cmd;
    app->onInputEvent = handle_input;

    int events;
    struct android_poll_source *pSource;
    while(!app->userData){

        if (ALooper_pollAll(0, NULL, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(app, pSource);
            }
        }
    }
    init_engine_memory();

    assets_manager = app->activity->assetManager;

    init_audio_manager();

    draw_loading_screen();
    eglSwapBuffers(display,surface);


    init_engine();

    init_game_engine();

    int level_result = load_level_to_elements_array("levels/test.lvl", actual_elements_array);
    if(level_result != 0){
        raise(SIGINT);
        return;
    }

    load_gui("gui/test.gui");

    for(int i = 0; i< texts.count ; i++){
        load_and_create_simple_model(get_from_array(&texts,i));
    }

    for(int i = 0; i< textures_paths.count ; i++){
        load_simple_image(get_from_array(&textures_paths,i));
    }


    init_game();

    while(!should_close){

        int events;
        struct android_poll_source *pSource;
        if (ALooper_pollAll(0, NULL, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(app, pSource);
            }
        }

        update_game();
        engine_loop();

        eglSwapBuffers(display,surface);


    }


}