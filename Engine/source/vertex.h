#ifndef VERTEX_H
#define VERTEX_H

#include "third_party/cglm/vec3.h"

typedef struct Vertex{
    vec3 postion;
    vec2 uv;
    vec4 weight;
    vec4 joint;
}Vertex;

#endif // !VERTEX_H

