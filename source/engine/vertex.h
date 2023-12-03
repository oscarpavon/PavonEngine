#ifndef VERTEX_H
#define VERTEX_H

#include "../ThirdParty/cglm/cglm.h"

typedef struct Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
    vec4 joint;
    vec4 weight;
    long unsigned int id;
    bool selected;
}Vertex;

#endif // !VERTEX_H

