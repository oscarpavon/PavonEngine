#ifndef MODEL_H
#define MODEL_H

#include "utils.h"

#include <GLES2/gl2.h>

#include "../ThirdParty/cglm/cglm.h"

#include "images.h"

#include "array.h"

#ifdef LINUX
#include <engine/renderer/vulkan/vulkan.h>
#endif

#include <engine/renderer/material.h>


typedef struct PMesh{
  GLuint vertex_buffer_id;
  GLuint index_buffer_id;
  Array vertex_array;
  Array index_array;

#ifdef LINUX
  VkBuffer vertex_buffer;
  VkBuffer index_buffer;
#endif
}PMesh;

typedef struct Model{
    int id;
    Array vertex_array;
    Array index_array;
    vec3 min;
    vec3 max;
    
    GLuint vertex_buffer_id;
    GLuint index_buffer_id;

    mat4 model_mat;
    GLuint shader;

    Texture texture;
	bool gpu_ready;
    PMaterial material;

#ifdef LINUX
    VkBuffer vertex_buffer;
    VkBuffer index_buffer;
#endif
    vec3 position;
    PMesh mesh;
}Model;

typedef struct DrawData{
    u32 shader;
    u32 texture;
    u32 vertex;
    u32 index;
}DrawData;

Model* pe_vk_model_load(char* path);

int pe_loader_model(const char* path);


#endif // !MODEL_H
