#ifndef MODEL_H
#define MODEL_H

#include "utils.h"
#include "vector.h"
#include <GLES2/gl2.h>

#include <cglm.h>

#include "images.h"
struct Model{
    VertexArray vertex_array;
    IndexArray index_array;

    GLuint vertex_buffer_id;
    GLuint index_buffer_id;

    mat4 model_mat;
    GLuint shader;

    Texture texture;
};

struct Geometry{

};


void init_model();
void load_model(const char* path, struct Model*);

#endif // !MODEL_H