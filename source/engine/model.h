#ifndef MODEL_H
#define MODEL_H

#include "utils.h"

#include <GLES2/gl2.h>

#include "../ThirdParty/cglm/cglm.h"

#include "images.h"

#include "skeletal.h"

#include "array.h"

typedef struct SkinnedMeshComponent SkinnedMeshComponent;

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

SkinnedMeshComponent* pe_curr_skin_loading;

#endif // !MODEL_H
