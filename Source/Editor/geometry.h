#ifndef ENGINE_GEOMETRY
#define ENGINE_GEOMETRY

#include "../vertex.h"
/* Create indices in current_model->vertex_array */
void geometry_cube_create_vertices( float* box );

/* Create indices in current_model->index_array */
void geometry_cube_create_indices();

void geometry_sphere_create_vertices(int);

#endif