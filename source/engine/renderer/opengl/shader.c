//
// Created by pavon on 6/24/19.
//

#include "shader.h"
#include <engine/file_loader.h>
#include <engine/threads.h>

void pe_shader_get_error(GLuint shader, GLenum info_type,
                         const char *path_for_error_debug) {

  char *status_description;
  GLint passed = GL_TRUE;
  GLint lenght;
  if (info_type == GL_COMPILE_STATUS) {
    glGetShaderiv(shader, info_type, &passed);
    if (passed == GL_FALSE) {
			status_description = "Compile fail";
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
			GLchar error[lenght];
			glGetShaderInfoLog(shader, lenght, &lenght, &error[0]);
      LOG("%s shader: %s\n", status_description, path_for_error_debug);
			LOG("Error Compiling: %s\n", error);
    }
  }
  if (info_type == GL_LINK_STATUS) {
    glGetProgramiv(shader, info_type, &passed);
    if (passed == GL_FALSE) {
			status_description = "Link fail";
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &lenght);
			GLchar error[lenght];
			glGetProgramInfoLog(shader, lenght, &lenght, error);
      LOG("%s shader: %s\n", status_description, path_for_error_debug);
			LOG("Error Linking: %s\n", error);
    }
  }
}

GLuint compile_shader(const char* src , GLenum type){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1,&src,0);

    glCompileShader(shader);

    char* error_description;
    if(type == GL_FRAGMENT_SHADER){
        error_description = "fragment shader";
    }else if (type == GL_VERTEX_SHADER){
        error_description = "vertex shader";
    }

    pe_shader_get_error(shader,GL_COMPILE_STATUS,error_description);
    return shader;
}

GLuint load_shader_file(const char* path, GLenum shader_type){
    File new_file;
    load_file(path,&new_file);
    GLuint shader = compile_shader(new_file.data,shader_type);
    return shader;
}

void pe_shader_compile_std(){
    standart_vertex_shader = compile_shader(triVertShader, GL_VERTEX_SHADER);
    standart_fragment_shader = compile_shader(triFragShader, GL_FRAGMENT_SHADER);
    shader_source_color_fragment_shader = compile_shader(color_shader_src,GL_FRAGMENT_SHADER);

		shader_skin_vertex = load_shader_file(
      "/home/pavon/PavonEngine/NativeContent/shaders/skin_vertex_shader.glsl",
      GL_VERTEX_SHADER);
}

GLuint create_engine_shader(GLuint vertex, GLuint fragment){
    shader_count++;
    GLuint new_shader = glCreateProgram();
    glAttachShader(new_shader, vertex);
    glAttachShader(new_shader, fragment);
    glLinkProgram(new_shader);
		pe_shader_get_error(new_shader,GL_LINK_STATUS,"Link status");
		glUseProgram(new_shader);
    return new_shader;
}

void pe_shader_create(PEShaderCreation* creation){
	thread_main.wait = true;
	creation->model->shader = create_engine_shader(creation->vertex,creation->pixel);	
	LOG("Shader creating id: %i\n",creation->model->shader);
	thread_main.wait = false;
}
