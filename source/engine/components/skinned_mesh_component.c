#include "skinned_mesh_component.h"
#include <engine/engine.h>
void pe_comp_skinned_mesh_init(ComponentDefinition* element_component){

  SkinnedMeshComponent *mesh_component = element_component->data;
  new_empty_model();
  Model *original_model =
      array_get(&array_models_loaded, array_models_loaded.count - 1);

  duplicate_model_data(selected_model, original_model);

  PEShaderCreation shader_creation;
  ZERO(shader_creation);
  shader_creation.model = selected_model;
  shader_creation.vertex = shader_skin_vertex;
  shader_creation.pixel = standart_fragment_shader;

	thread_main.wait = true;

  pe_th_exec_in(pe_th_render_id, &pe_shader_create, &shader_creation);

	pe_th_wait(&thread_main); 

  glm_mat4_copy(element_component->parent->transform->model_matrix,
                selected_model->model_mat);

  mesh_component->mesh = selected_model;

  memcpy(&mesh_component->joints, &model_nodes, sizeof(Array));

  memcpy(&mesh_component->animations, &model_animation, sizeof(Array));

  memcpy(mesh_component->inverse_bind_matrices,
         model_loaded_inverse_bind_matrices,
         sizeof(model_loaded_inverse_bind_matrices));

  mesh_component->node_uniform.joint_count = mesh_component->joints.count;

  update_skeletal_node_uniform();
}
