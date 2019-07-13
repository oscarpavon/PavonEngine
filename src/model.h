#ifndef MODEL_H
#define MODEL_H

#include "utils.h"
#include "vector.h"
#include <GLES2/gl2.h>

#include <cglm.h>

#include "images.h"

#include "skeletal.h"

#include "vector.h"

typedef struct Model{
    int id;
    VertexArray vertex_array;
    IndexArray index_array;

    GLuint vertex_buffer_id;
    GLuint index_buffer_id;

    mat4 model_mat;
    GLuint shader;

    Texture texture;

    struct Skeletal* skeletal;
    
    unsigned int short LOD_count;
    unsigned int short actual_LOD;
    struct Model* LOD;
    bool change_LOD;
    struct Model* HLOD;
    bool has_HLOD;
    bool change_to_HLOD;
    
    bool draw;
}Model;

struct Geometry{
    VertexArray vertex_array;
};


void init_model();
int load_model(const char* path, struct Model*);


#endif // !MODEL_H