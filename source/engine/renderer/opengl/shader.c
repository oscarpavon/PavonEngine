//
// Created by pavon on 6/24/19.
//

#include "shader.h"
#include <engine/file_loader.h>

void print_shader_compile_log(GLuint shader, const char* path_for_error_debug){

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        LOG("Failed to compile shader: %s\n",path_for_error_debug);
        GLint lenght;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
        GLchar error[lenght];
        glGetShaderInfoLog(shader, lenght, &lenght, &error[0]);
        LOG("Error: %s\n",error);
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

GLuint load_shader_file(const char* path, GLenum shader_type){
    File new_file;
    load_file(path,&new_file);
    GLuint shader = compile_shader(new_file.data,shader_type);
    return shader;
}

void shader_compile_standard_shaders(){
    standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    shader_source_color_fragment_shader = compile_shader(color_shader_src,GL_FRAGMENT_SHADER);
}
