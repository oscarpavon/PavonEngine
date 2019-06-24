//
// Created by pavon on 6/24/19.
//

#include "shader.h"


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