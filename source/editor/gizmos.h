/* created by pavon on 2019-07-09 */
#ifndef EDITOR_GIZMOS_H
#define EDITOR_GIZMOS_H

#include "editor.h"
#include"../engine/Math/sphere.h"

typedef struct Grid{
    bool initialized;
    vec2 size;    
    Array vertex_array;
    GLuint vertex_buffer_id;
    GLuint shader;
}Grid;

typedef struct DebugLine{
    bool initialized;
    vec3 start;
    vec3 end;
    float duration;
    vec4 color;    
    Array vertex_array;
    GLuint vertex_buffer_id;
    GLuint shader;
}DebugLine;

void draw_gizmos();
void gizmos_init();


void gizmos_boanding_sphere_draw(Sphere* sphere, vec4 color);

bool can_draw_gizmos;
bool draw_translate_gizmo;
bool draw_rotate_gizmo;
bool can_draw_skeletal_bones;
bool can_draw_bounding_box_in_select_element;
bool gizmos_draw_scale;

bool gizmos_can_draw_spheres;

Array gizmos;

Element* selected_camera;


#endif
