#include "skinned_mesh_component.h"
#include <engine/engine.h>
void pe_comp_skinned_mesh_init(ComponentDefinition* element_component){

  SkinnedMeshComponent *mesh_component = element_component->data;
  new_empty_model();
  Model *original_model =
      array_get(&array_models_loaded, array_models_loaded.count - 1);

  duplicate_model_data(selected_model, original_model);
  GLuint skin_vertex_shader = load_shader_file(
      "/home/pavon/PavonEngine/NativeContent/shaders/skin_vertex_shader.glsl",
      GL_VERTEX_SHADER);

	selected_model->shader =
      create_engine_shader(skin_vertex_shader, standart_fragment_shader);

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
