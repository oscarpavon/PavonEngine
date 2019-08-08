#ifndef SKELETAL_EDITOR_H
#define SKELETAL_EDITOR_H

#include "../skeletal.h"

void draw_skeletal_bones();
void init_skeletal_editor();

void init_skeletal_gizmo();
void update_joints_vertex();

void update_skeletal_vertices_gizmo(mat4 global, int i, Node* current_joint);
void clear_skeletal_vertices();
#endif // !SKELETAL_EDITOR_H