

#include <EGL/egl.h>
#include <GLES2/gl2.h>



#include "third_party/stb_image.h"

#include "vector.h"
#include "model.h"
#include "level.h"
#include "camera.h"

#include <vec3.h>
#include <cglm.h>

#include "utils.h"
#include "file_loader.h"

#include "images.h"

#include "audio.h"
#include "input.h"
#include "gui.h"

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

bool should_close = false;

static const char* triVertShader = "attribute vec3 vPosition;\
attribute vec2 inUV;\
varying vec2 v_TexCoord; \
uniform mat4 MVP;   \
void main()\
{\
    gl_Position = MVP * vec4(vPosition,1);\
    v_TexCoord = inUV;\
}\
//end";

static const char*  triFragShader = "precision mediump float; \
   uniform sampler2D texture_sampler;\
   varying vec2 v_TexCoord;                     \
    void main()\
    {\
        gl_FragColor = texture2D(texture_sampler, v_TexCoord);\
    }\
//end";

void print_shader_compile_log(GLuint shader, const char* path_for_error_debug){

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        LOGW("Failed to compile shader: %s\n",path_for_error_debug);
        GLint lenght;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
        GLchar error[lenght];
        glGetShaderInfoLog(shader, lenght, &lenght, &error[0]);
        LOGW("%s\n",error);
    }

}


GLuint compile_shader(const char* src , GLenum type){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1,&src,0);

    glCompileShader(shader);

    char* error_description = "";
    if(type == GL_FRAGMENT_SHADER){
        error_description = "fragment shader";
    }else if (type == GL_VERTEX_SHADER){
        error_description = "vertex shader";
    }

    print_shader_compile_log(shader,error_description);
    return shader;
}


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

    LOGW("---------> Windows size: %f %f",(float)w,(float)h);

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

Level new_level;

void init_level_models(){
    for(size_t i = 0; i < new_level.models_array.count ; i++){

        struct Model* new_model = &new_level.models_array.models[i];

        VertexArray vertex_array = new_model->vertex_array;


        glGenBuffers(1,&new_model->vertex_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, vertex_array.count * sizeof(struct Vertex) , vertex_array.vertices, GL_STATIC_DRAW);


        glGenBuffers(1,&new_model->index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     new_model->index_array.count * sizeof(unsigned short int),
                     new_model->index_array.indices , GL_STATIC_DRAW);

        free(new_model->vertex_array.vertices);
        free(new_model->index_array.indices);
    }

}

void create_models_shaders(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {
        struct Model* new_model = &new_level.models_array.models[i];

        GLuint vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
        GLuint frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);

        new_model->shader = glCreateProgram();
        glAttachShader(new_model->shader, vert_shader);
        glAttachShader(new_model->shader, frag_shader);
        glLinkProgram(new_model->shader);

    }

    //button shader
    GLuint vert_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    GLuint frag_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);

    button1.shader = glCreateProgram();
    glAttachShader( button1.shader, vert_shader);
    glAttachShader( button1.shader, frag_shader);
    glLinkProgram( button1.shader);

}
void load_model_texture_to_gpu(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {
        struct Model *model = &new_level.models_array.models[i];


        glGenTextures(1, &model->texture.id);
        glBindTexture(GL_TEXTURE_2D, model->texture.id);

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, model->texture.image.width,
                        model->texture.image.heigth, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, model->texture.image.pixels_data);

        free_image(&model->texture.image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        GLenum error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("texture error \n");
            LOGW("Error %08x \n",error);
        }

    }
}

static inline void update_mvp(mat4 model, mat4 mvp_out){
    mat4 projection_view;
    glm_mul(main_camera.projection , main_camera.view, projection_view);
    glm_mul(projection_view , model , mvp_out);

}

void draw_frame(){
    for(size_t i = 0; i < new_level.models_array.count ; i++) {

        struct Model *new_model = &new_level.models_array.models[i];

        glUseProgram(new_model->shader);
        glBindTexture(GL_TEXTURE_2D, new_model->texture.id);

        mat4 mvp;
        glm_mat4_identity(mvp);

        //glm_rotate(&new_level.models_array.models[2].model_mat, 0.005f, (vec3){0,0,1});
        update_mvp(new_model->model_mat, mvp);

        GLint mvp_uniform =  glGetUniformLocation(new_model->shader,"MVP");
        //glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &new_model->model_mat[0][0]);
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

        glBindBuffer(GL_ARRAY_BUFFER,new_model->vertex_buffer_id);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, uv));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,new_model->index_buffer_id);

        glDrawElements(GL_TRIANGLES, new_model->index_array.count , GL_UNSIGNED_SHORT, (void*)0);

        GLenum error = glGetError();
        if(error != GL_NO_ERROR){
            LOGW("draw error\n");
            LOGW("Error %08x \n",error);
        }
    }

}



void android_main(struct android_app* app){
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

    assets_manager = app->activity->assetManager;

    init_audio_manager();




    load_level("level01.lvl",&new_level);

    init_camera();

    init_button();

    glEnable(GL_DEPTH_TEST);


    load_model_texture_to_gpu();
    create_models_shaders();
    init_level_models();

    while(!should_close){

        if (ALooper_pollAll(0, NULL, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(app, pSource);
            }
        }

        glClearColor(1,0.5,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //glm_rotate(mvp, 0.005f, axis);

        draw_frame();
        draw_button();

        eglSwapBuffers(display,surface);

    }

}