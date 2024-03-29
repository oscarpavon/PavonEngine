#ifndef PE_COMPONENT_SKINNED
#define PE_COMPONENT_SKINNED
#include "components.h"
#include <engine/model.h>
#include <engine/skeletal.h>
typedef struct PSkinnedMeshComponent {
  Array meshes;
  Array distances;
  Array textures;
  PModel *mesh;
  Array joints;
  vec3 bounding_box[2];
  Array animations;
  Array inverse_bind_matrices;
  struct SkeletalNodeUniform node_uniform;
  TransformComponent *transform;
  Array shader_storage_buffers;
  Array shader_storage_buffers_memory;
} PSkinnedMeshComponent;

void pe_comp_skinned_mesh_init(ComponentDefinition *);

PSkinnedMeshComponent *pe_curr_skin_loading;

#endif
