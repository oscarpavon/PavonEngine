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

static inline void get_local_matrix(Node* node, mat4 out_mat);
static inline void get_global_matrix(Node* node, mat4 out_mat);


#endif