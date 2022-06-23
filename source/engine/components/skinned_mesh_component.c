#include "skinned_mesh_component.h"
#include <engine/engine.h>
void pe_comp_skinned_mesh_init(ComponentDefinition* element_component){

  SkinnedMeshComponent *mesh_component = element_component->data;
  new_empty_model();

	Model *original_model = array_pop(&array_models_loaded);

  duplicate_model_data(selected_model, original_model);

  PEShaderCreation shader_creation;
  ZERO(shader_creation);
  //shader_creation.vertex = shader_skin_vertex;//TODO:Android 
  shader_creation.vertex = standart_vertex_shader;

  shader_creation.pixel = standart_fragment_shader;
	shader_creation.shader = &selected_model->shader;

  pe_th_exec_in(pe_th_render_id, &pe_shader_new, &shader_creation);

	pe_th_wait(&thread_main); 

  glm_mat4_copy(element_component->parent->transform->model_matrix,
                selected_model->model_mat);

	memcpy(mesh_component,pe_curr_skin_loading,sizeof(SkinnedMeshComponent));

	mesh_component->transform = get_component_from_element(element_component->parent,TRASNFORM_COMPONENT);  
  mesh_component->mesh = selected_model;
	mesh_component->node_uniform.joint_count = mesh_component->joints.count;

	pe_curr_skin_loading = NULL;//for new skin loading

  pe_anim_nodes_update(mesh_component);
}
