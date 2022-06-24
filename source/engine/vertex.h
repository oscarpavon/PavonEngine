#ifndef VERTEX_H
#define VERTEX_H

#include "../ThirdParty/cglm/cglm.h"

typedef struct Vertex{
    vec3 postion;
    vec3 normal;
    vec2 uv;
    vec4 joint;
    vec4 weight;
}Vertex;

#endif // !VERTEX_H

