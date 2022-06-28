#include "node.h"
#include <engine/skeletal.h>
#include <engine/components/skinned_mesh_component.h>
#include <engine/animation/animation.h>


void pe_node_rotate(Node* node, float angle, vec3 axis){


    versor new_rot_quat;
    
    glm_quatv(new_rot_quat, glm_rad(angle), axis);

    versor result_quat;
    glm_quat_mul(node->rotation, new_rot_quat, result_quat);

    glm_quat_copy(result_quat, node->rotation);

    //mat4 model_rot_mat;
    //glm_quat_mat4(new_rot_quat,model_rot_mat);
   
#ifdef DEBUG
    update_vertex_bones_gizmos = true;
#endif
    


}

Node* pe_node_by_name(Array* array, const char* name){
  for( int i = 0; i < array->count ; i++ ){
    Node* node = array_get(array,i);
    if( strcmp( node->name , name ) == 0){
      return node;
    }
  }
}



