#ifndef MODEL_H
#define MODEL_H

#include "utils.h"

#include <GLES2/gl2.h>

#include "../ThirdParty/cglm/cglm.h"

#include "images.h"

#include "skeletal.h"

#include "array.h"

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
}Model;

typedef struct DrawData{
    u32 shader;
    u32 texture;
    u32 vertex;
    u32 index;
}DrawData;

int pe_loader_model(const char* path);

int model_load_from_content(void* gltf_data, u32 size);

//SKINNED MESH
Array model_nodes;
Array model_animation;
mat4 model_loaded_inverse_bind_matrices[50];
//Skeletal loaded_skeletal;
#endif // !MODEL_H
