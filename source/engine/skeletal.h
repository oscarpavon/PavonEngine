#ifndef PAVON_ENGINE_SKELETAL_H
#define PAVON_ENGINE_SKELETAL_H


#include "../ThirdParty/cglm/cglm.h"

#include "array.h"
#include "model.h"

struct Node;
typedef struct Node{
    int id;
    char name[50];
    struct Node* parent;
    vec3 translation;
    versor rotation;
}Node;

typedef struct Skeletal{
    Node* joints;
    unsigned short int joints_count;
}Skeletal;

struct SkeletalNodeUniform{
    mat4 joints_matrix[50];
    unsigned short int joint_count;
};

void free_node(Node*);

void get_local_matrix(Node* node, mat4 out_mat);
void get_global_matrix(Node* node, mat4 out_mat);


#endif
