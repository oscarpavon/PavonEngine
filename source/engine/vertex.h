#ifndef VERTEX_H
#define VERTEX_H

#include "../ThirdParty/cglm/cglm.h"

typedef struct Vertex{
    vec3 position;
    vec2 uv;
    vec3 color;
    vec3 normal;
    vec4 joint;
    vec4 weight;
    long unsigned int id;
    bool selected;
}Vertex;

#endif // !VERTEX_H

