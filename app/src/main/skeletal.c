#include "skeletal.h"

static inline void get_local_matrix(Node* node, mat4 out_mat){
    mat4 translation;
    glm_translate(translation,node->translation);
    mat4 rot;
    glm_quat_mat4(node->rotation,rot);
    glm_mul(translation,rot,out_mat);
}

static inline void get_global_matrix(Node* node, mat4 out_mat){
    mat4 local;
    get_local_matrix(node, local);
    Node* node_parent = node->parent;
    while(node_parent != NULL){
        mat4 local_mat_parent;
        get_local_matrix(node_parent, local_mat_parent);
        glm_mul(local_mat_parent,local,out_mat);
        node_parent = node_parent->parent;
    }
}
