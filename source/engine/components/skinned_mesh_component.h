#ifndef PE_COMPONENT_SKINNED
#define PE_COMPONENT_SKINNED
#include "components.h"

typedef struct SkinnedMeshComponent{    
    Array meshes;
    Array distances;
    Array textures;
    Model* mesh;
    Array joints;
    vec3 bounding_box[2];
    Array animations;
    mat4 inverse_bind_matrices[50];
    struct SkeletalNodeUniform node_uniform;
		TransformComponent* transform;
}SkinnedMeshComponent;

void pe_comp_skinned_mesh_init(ComponentDefinition*);

#endif
