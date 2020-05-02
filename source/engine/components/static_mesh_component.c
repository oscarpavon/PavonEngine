#include "static_mesh_component.h"
#include <engine/engine.h>
#include "components.h"

#ifdef EDITOR
#include "../../editor/editor.h"
#endif

void pe_comp_static_mesh_update(ComponentDefinition* element_component){

  StaticMeshComponent *mesh_component = element_component->data;

  for (u8 i = 1; i <= mesh_component->meshes.count - 1; i++) {
    u8 *id = array_get(&mesh_component->meshes, i);
    Model *model = array_get(actual_model_array, *id);
    glm_mat4_copy(element_component->parent->transform->model_matrix,
                  model->model_mat);
  }

  u8 *id = array_get(&mesh_component->meshes, 1);
  Model *model = array_get(actual_model_array, *id);
  if (!model)
    return;
  glm_vec3_copy(model->min, mesh_component->bounding_box[0]);
  glm_vec3_copy(model->max, mesh_component->bounding_box[1]);
  glm_aabb_transform(mesh_component->bounding_box,
                     element_component->parent->transform->model_matrix,
                     mesh_component->bounding_box);
  glm_aabb_center(mesh_component->bounding_box, mesh_component->center);
}

void pe_comp_static_mesh_init(ComponentDefinition* element_component){

  StaticMeshComponent *mesh_component = element_component->data;

  for (u32 i = 1; i <= mesh_component->meshes.count - 1; i++) {

    u8 *id = array_get(&mesh_component->meshes, i);

    Model *original_model = array_get(&array_models_loaded, *id);
    if (!original_model)
      return;

    new_empty_model();

    duplicate_model_data(selected_model, original_model);
   
		PEShaderCreation shader_creation;
		ZERO(shader_creation);
		shader_creation.model = selected_model;
		shader_creation.vertex = standart_vertex_shader;
		shader_creation.pixel = standart_fragment_shader;
		
		pe_th_exec_in(pe_th_render_id,&pe_shader_create,&shader_creation);

		thread_main.wait = true;
		pe_th_wait(&thread_main); 

		u8 *texture_id = NULL;
    if (mesh_component->textures.count > 0)
      texture_id = array_get(&mesh_component->textures, i);

    if (texture_id) {
      Texture *texture = array_get(current_textures_array, *texture_id);
      if (texture)
        selected_model->texture.id = texture->id;
    } else {
#ifdef EDITOR
      selected_model->texture.id = editor_texture_checker.id;
#endif
    }

    glm_mat4_copy(element_component->parent->transform->model_matrix,
                  selected_model->model_mat);
  }

  u8 id = actual_model_array->count - (mesh_component->meshes.count - 1);
  for (u8 i = 1; i <= mesh_component->meshes.count - 1; i++) {
    u8 *geted_id = array_get(&mesh_component->meshes, i);
    memcpy(geted_id, &id, sizeof(u8));
    id++;
  }

  update_component(element_component);
}
