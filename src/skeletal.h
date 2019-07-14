#ifndef PAVON_ENGINE_SKELETAL_H
#define PAVON_ENGINE_SKELETAL_H

#ifdef ANDROID
#include <cglm.h>
#else
#include <cglm/cglm.h>
#endif // ANDROID

#include "vector.h"
#include "model.h"

struct Node;
typedef struct Node{
    char name[15];
    struct Node* parent;
    vec3 translation;
    versor rotation;
}Node;

typedef struct Skeletal{
    Node* joints;
    unsigned short int joints_count;
}Skeletal;


void free_node(Node*);

void get_local_matrix(Node* node, mat4 out_mat);
void get_global_matrix(Node* node, mat4 out_mat);


#endif