#ifndef OPENGL_ES_2_RENDERER_H
#define OPENGL_ES_2_RENDERER_H



void draw_model_with_color(Model* model, GLenum mode, vec4 color);
void draw_model_like(Model* model, GLenum mode);

void load_texture_to_GPU(Texture*);


static inline void mvp_error(const char* uniform_name){
    LOG("Uniform not found: %s\n",uniform_name);
    debug_break();
}


static inline void check_send_matrix_error(const char* message){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("[X] Send %s matrix error, Error %08x \n", message, error);
    }
}

static inline void check_error(const char* message){
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("%s, Error %08x \n", message, error);
    }
}

static inline GLint get_uniform_location(GLuint shader, const char* name){
    GLint uniform = glGetUniformLocation(shader,name);
    if(uniform == -1){
        mvp_error(name);
    }
    return uniform;
}
#endif // !OPENGL_ES_2_RENDERER_H