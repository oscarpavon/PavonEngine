/* created by pavon on 2019-07-09 */
#ifndef EDITOR_GIZMOS_H
#define EDITOR_GIZMOS_H

#include "editor.h"
#include"../Engine/Math/sphere.h"

void draw_gizmos();
void init_gizmos();


void gizmos_boanding_sphere_draw(Sphere* sphere, vec4 color);

bool can_draw_gizmos;
bool draw_translate_gizmo;
bool draw_rotate_gizmo;
bool can_draw_skeletal_bones;
bool can_draw_bounding_box_in_select_element;

Array gizmos;

Element* selected_camera;

#endif