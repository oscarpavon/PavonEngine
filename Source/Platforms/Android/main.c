#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "../../ThirdParty/cglm/cglm.h"

#include "../../Engine/utils.h"

#include "audio.h"
#include "../../Engine/camera.h"

#include "../../Engine/engine.h"
#include "input.h"
#include "../../Engine/file_loader.h"
#include "../../Engine/gui.h"
#include "../../Engine/level.h"
#include "../../Engine/game.h"

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


void android_init_window(struct android_app * app){


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

            android_init_window(pApp);


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

   /*  init_audio_manager();

    draw_loading_screen();
    eglSwapBuffers(display,surface);


    engine_init();

    init_game_engine();

    int level_result = load_level_to_elements_array("levels/test.lvl", actual_elements_array);
    if(level_result != 0){
        debug_break();
        return;
    }

    load_gui("gui/test.gui");

    for(int i = 0; i< texts.count ; i++){
        load_and_initialize_simple_model(array_get(&texts,i));
    }

    for(int i = 0; i< textures_paths.count ; i++){
        load_simple_image(array_get(&textures_paths,i));
    }


    init_game(); */

    engine_init();
    
    while(!should_close){

        int events;
        struct android_poll_source *pSource;
        if (ALooper_pollAll(0, NULL, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(app, pSource);
            }
        }

        /* update_game();
        engine_loop(); */
        glClearColor(1,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        text_render_in_screen_space(12, "Android Test", 50, 50);
        eglSwapBuffers(display,surface);


    }


}
