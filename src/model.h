#ifndef MODEL_H
#define MODEL_H

#include "utils.h"

#include <GLES2/gl2.h>

#include <cglm.h>

#include "images.h"

#include "skeletal.h"

#include "Engine/array.h"

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

}Model;

struct LoadGeometry{
    Array vertex_array;
    Array index_array;
};

int load_model(const char* path, struct Model*);


#endif // !MODEL_H