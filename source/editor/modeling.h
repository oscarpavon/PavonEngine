#ifndef MODELING_H
#define MODELING_H

#include<engine/array.h>
#include <engine/vertex.h>

typedef struct PVertexModeling{
  u32 id;
  Vertex* vertex;

}PVertexModeling;

void vertex_create();

void draw_vertices();

void init_modeling();

void pe_modeling_extrude_vertex(Array* vertex_array, Vertex* vertex);

Vertex* pe_modeling_get_vertex_by_id(Array* array , u64 id);

void pe_modeling_select_next_vertex();

void vertex_new(float x , float y , float z);

#endif
