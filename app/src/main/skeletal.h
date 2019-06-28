#ifndef PAVON_ENGINE_SKELETAL_H
#define PAVON_ENGINE_SKELETAL_H

#include <cglm.h>
#include "vector.h"
#include "model.h"

struct Node;
typedef struct Node{
    char* name;
    struct Node* parent;
    vec3 translation;
    versor rotation;
}Node;

typedef struct Skeletal{
    Node* nodes;
    Node* joints;

}Skeletal;


void free_node(Node*);

#endif