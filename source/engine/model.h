#ifndef MODEL_H
#define MODEL_H

#include "utils.h"

#include <GLES2/gl2.h>

#include "../ThirdParty/cglm/cglm.h"

#include "images.h"

#include "skeletal.h"

#include "array.h"

#include <engine/renderer/vulkan/vulkan.h>
#include <engine/renderer/material.h>
typedef struct SkinnedMeshComponent SkinnedMeshComponent;

typedef struct PMesh{
  GLuint vertex_buffer_id;
  GLuint index_buffer_id;
  Array vertex_array;
  Array index_array;
  VkBuffer vertex_buffer;
  VkBuffer index_buffer;
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
    VkBuffer vertex_buffer;
    VkBuffer index_buffer;

    vec3 position;

    VkDescriptorSet descriptor_set;
    PMaterial material;
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

SkinnedMeshComponent* pe_curr_skin_loading;

#endif // !MODEL_H
