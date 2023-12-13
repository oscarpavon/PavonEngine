#include "skeletal_editor.h"
#include "../engine/engine.h"
#include "../engine/shader.h"
#include "editor.h"

#include "../engine/components/components.h"
#include "../engine/vertex.h"
#include <engine/renderer/renderer.h>

PModel skeletal_gizmo;

void update_joints_vertex() {
  update_gpu_vertex_data(&skeletal_gizmo.vertex_array,
                         skeletal_gizmo.vertex_buffer_id);
}

void clear_skeletal_vertices() { array_clean(&skeletal_gizmo.vertex_array); }

void pe_skeletal_editor_creates_indices(int i, Node *current_joint) {

  if (current_joint->parent != NULL) {
    if (i == 2) {
      int id = i - 1;
      array_add(&skeletal_gizmo.index_array, &id);
    } else if (i >= 3) {
      array_add(&skeletal_gizmo.index_array, &current_joint->parent->id);
    }
  }
  array_add(&skeletal_gizmo.index_array, &i);
}

void init_skeletal_vertices(mat4 global, int i, Node *current_joint) {
  //    struct Vertex vert = { { global[3][0],global[3][1],global[3][2] }
  //    ,{0,0}};
  PVertex new_vertex;
  ZERO(new_vertex);

  glm_vec3_copy(global[3], new_vertex.position);

  array_add(&skeletal_gizmo.vertex_array, &new_vertex);

  pe_skeletal_editor_creates_indices(i, current_joint);

  LOG("Created vertices for: %s\n", current_joint->name);
}

void pe_skeletal_update_draw_vertices_target(
    PSkinnedMeshComponent *skin_component, mat4 target_model_mat) {

  array_clean(&skeletal_gizmo.vertex_array);
  array_clean(&skeletal_gizmo.index_array);

  for (int i = 0; i < skin_component->joints.count; i++) {

    Node *joint = (Node *)array_get(&skin_component->joints, i);

    mat4 local;
    get_global_matrix(joint, local);
    mat4 global;
    glm_mat4_mul(skin_component->transform->model_matrix, local, global);

    init_skeletal_vertices(global, i, joint);
  }

  update_vertex_bones_gizmos = true;
}
void pe_skeletal_update_draw_vertices(PSkinnedMeshComponent *skin_component) {

  array_clean(&skeletal_gizmo.vertex_array);
  array_clean(&skeletal_gizmo.index_array);

  for (int i = 0; i < skin_component->joints.count; i++) {

    Node *joint = (Node *)array_get(&skin_component->joints, i);

    mat4 local;
    get_global_matrix(joint, local);
    mat4 global;
    glm_mat4_mul(skin_component->transform->model_matrix, local, global);

    init_skeletal_vertices(global, i, joint);
  }

  update_vertex_bones_gizmos = true;
}
void update_skeletal_vertices_gizmo(mat4 global, int i, Node *current_joint) {
  struct Vertex vert = {{global[3][0], global[3][1], global[3][2]}, {0, 0}};
  if (skeletal_gizmo.vertex_array.initialized) {
    array_add(&skeletal_gizmo.vertex_array, &vert);
  }
}

void pe_skeletal_editor_create_vertices(PSkinnedMeshComponent *skin_component) {
  if (!skin_component) {
    LOG("No skinned mesh component\n");
    return;
  }

  int vertex_count = skin_component->joints.count;
  struct Vertex vertices[vertex_count];
  ZERO(vertices);

  array_init(&skeletal_gizmo.vertex_array, sizeof(PVertex),
             skin_component->joints.count);

  // joints index for vertex
  int index = 0;
  for (int i = 0; i < skin_component->joints.count; i++) {
    Node *joint = (Node *)array_get(&skin_component->joints, i);
    joint->id = index;
    index++;
  }

  array_init(&skeletal_gizmo.index_array, sizeof(unsigned short int), 100);

  for (int i = 0; i < skin_component->joints.count; i++) {

    Node *joint = (Node *)array_get(&skin_component->joints, i);

    mat4 local;
    get_global_matrix(joint, local);
    mat4 global;
    glm_mat4_mul(skin_component->transform->model_matrix, local, global);

    init_skeletal_vertices(global, i, joint);
  }
}
void create_skeletal_vertices() {
  PSkinnedMeshComponent *skin_component = pe_comp_get(COMPONENT_SKINNED_MESH);
  if (!skin_component) {
    LOG("No skinned mesh component\n");
    return;
  }

  int vertex_count = skin_component->joints.count;
  struct Vertex vertices[vertex_count];
  ZERO(vertices);

  array_init(&skeletal_gizmo.vertex_array, sizeof(PVertex),
             skin_component->joints.count);

  // joints index for vertex
  int index = 0;
  for (int i = 0; i < skin_component->joints.count; i++) {
    Node *joint = (Node *)array_get(&skin_component->joints, i);
    joint->id = index;
    index++;
  }

  array_init(&skeletal_gizmo.index_array, sizeof(unsigned short int), 100);

  for (int i = 0; i < skin_component->joints.count; i++) {

    Node *joint = (Node *)array_get(&skin_component->joints, i);

    mat4 local;
    get_global_matrix(joint, local);
    mat4 global;
    glm_mat4_mul(selected_element->transform->model_matrix, local, global);

    init_skeletal_vertices(global, i, joint);
  }
}

void pe_debug_skeletal_draw(Element *element) {
  selected_element = element;
  draw_skeletal_bones();
}

void draw_skeletal_bones() {
  if (update_vertex_bones_gizmos == true) {
    update_joints_vertex(); // on gpu
    LOG("######### joins vertex updated");
    update_vertex_bones_gizmos = false;
  }

  if (pe_bool_can_draw_skeletal == false) {
    return;
  }

  render_clear_buffer(RENDER_DEPTH_BUFFER);
  //   LOG("********Drawing skeletal gizmo");

  mat4 model;
  glm_mat4_identity(model);
  mat4 mvp;
  update_mvp(model, mvp);

  update_draw_vertices(skeletal_gizmo.shader, skeletal_gizmo.vertex_buffer_id,
                       mvp);

  glDrawArrays(GL_POINTS, 0, skeletal_gizmo.vertex_array.count);

  glLineWidth(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skeletal_gizmo.index_buffer_id);
  glDrawElements(GL_LINES, skeletal_gizmo.index_array.count, GL_UNSIGNED_SHORT,
                 (void *)0);
}

void pe_debug_skeletal_show_bones(Element *element) {
  selected_element = element;
  create_skeletal_vertices();
  init_static_gpu_vertex_buffer(&skeletal_gizmo.vertex_array,
                                &skeletal_gizmo.vertex_buffer_id);
  init_static_gpu_index_buffer(&skeletal_gizmo.index_array,
                               &skeletal_gizmo.index_buffer_id);

  skeletal_blue_shader =
      compile_shader(skeletal_blue_joint_source, GL_FRAGMENT_SHADER);

  skeletal_gizmo.shader =
      create_engine_shader(standart_vertex_shader, skeletal_blue_shader);
}

void pe_skeletal_editor_init_for(PSkinnedMeshComponent *skin) {

  LOG("########### Skeleltal editor init");

  pe_skeletal_editor_create_vertices(skin);

  glGenBuffers(1, &skeletal_gizmo.vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, skeletal_gizmo.vertex_buffer_id);
  glBufferData(GL_ARRAY_BUFFER,
               skeletal_gizmo.vertex_array.count * sizeof(PVertex),
               skeletal_gizmo.vertex_array.data, GL_DYNAMIC_DRAW);

  init_static_gpu_index_buffer(&skeletal_gizmo.index_array,
                               &skeletal_gizmo.index_buffer_id);

  skeletal_blue_shader =
      compile_shader(skeletal_blue_joint_source, GL_FRAGMENT_SHADER);

  skeletal_gizmo.shader =
      create_engine_shader(standart_vertex_shader, skeletal_blue_shader);

  pe_bool_can_draw_skeletal = true;
}

void init_skeletal_editor() {
  LOG("Skeletal editor\n");

  create_skeletal_vertices();

  glGenBuffers(1, &skeletal_gizmo.vertex_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, skeletal_gizmo.vertex_buffer_id);
  glBufferData(GL_ARRAY_BUFFER,
               skeletal_gizmo.vertex_array.count * sizeof(PVertex),
               skeletal_gizmo.vertex_array.data, GL_DYNAMIC_DRAW);

  init_static_gpu_index_buffer(&skeletal_gizmo.index_array,
                               &skeletal_gizmo.index_buffer_id);

  skeletal_blue_shader =
      compile_shader(skeletal_blue_joint_source, GL_FRAGMENT_SHADER);

  skeletal_gizmo.shader =
      create_engine_shader(standart_vertex_shader, skeletal_blue_shader);

  pe_bool_can_draw_skeletal = true;
}
