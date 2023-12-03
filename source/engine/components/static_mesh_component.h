#ifndef COMPONENT_STATIC_MESH
#define COMPONENT_STATIC_MESH
#include "components.h"
#include <engine/renderer/material.h>
#include <engine/model.h>
typedef struct StaticMeshComponent{
    Array meshes;
    Array distances;
    Array textures;
    Array models_p;
    vec3 center;
    vec3 bounding_box[2];
    PMaterial material;
    
}StaticMeshComponent;

void pe_comp_static_mesh_init(ComponentDefinition*);
void pe_comp_static_mesh_update(ComponentDefinition*);

void pe_comp_static_mesh_add_to_element();

void pe_comp_static_mesh_shader_init(PModel* model);

void pe_shader_create_for_model(PModel* model, u32 frag, u32 vert);

#endif
