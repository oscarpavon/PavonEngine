#ifndef OPENGL_ES_2_RENDERER_H
#define OPENGL_ES_2_RENDERER_H

#include <engine/model.h>
#include <engine/log.h>

#define RENDER_COLOR_BUFFER GL_COLOR_BUFFER_BIT
#define RENDER_DEPTH_BUFFER GL_DEPTH_BUFFER_BIT

void draw_vertices_like(GLenum mode, Model* model, vec4 color);
void draw_model_with_color(Model* model, GLenum mode, vec4 color);
void draw_model_like(Model* model, GLenum mode);
void pe_render_2d(DrawData* data, vec2 position , vec2 size, vec4 color);

void pe_tex_to_gpu(Texture*);

void GPU_buffers_create_for_model(Model* model);

void init_static_gpu_vertex_buffer(Array* array, GLuint *id);
void init_static_gpu_index_buffer(Array* array, GLuint *id);
void update_draw_vertices(GLuint shader, GLuint buffer, mat4 matrix);
void update_gpu_vertex_data(Array* array, GLuint id);

void pe_render_skinned_model(SkinnedMeshComponent * new_model);

static inline void mvp_error(const char* uniform_name){
    LOG("Uniform not found: %s\n",uniform_name);
  //  debug_break();
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

static inline void send_color_to_shader(u32 shader_id, vec4 color){
    GLint uniform_color = get_uniform_location(shader_id,"color");
    
    glUniform4fv(uniform_color, 1, color);
}

inline static void render_clear_buffer(int buffer_bits){	
    glClear(buffer_bits);
}
#endif // !OPENGL_ES_2_RENDERER_H
