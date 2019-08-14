#include "collection.h"
#include "../../engine.h"

void get_edges_axis(Model* model){
    u8 vertex_count = model->vertex_array.count;
    vec3 axis[vertex_count];
    vec3 edges[vertex_count];
    for (u8 i = 0; i < vertex_count; i=+3)
    {
        Vertex* vertex = get_from_array(&model->vertex_array,i);
        Vertex* vertex2 = get_from_array(&model->vertex_array,i+1);
        Vertex* vertex3 = get_from_array(&model->vertex_array,i+2);
        vec3 edge;
        glm_vec3_sub(vertex->postion,vertex2->postion,edge);
        vec3 edge2;
        glm_vec3_sub(vertex2->postion,vertex3->postion,edge2);
        vec3 surface_normal;
        glm_cross(edge,edge2,surface_normal);
        glm_normalize(surface_normal);

        vec3 edge1_normal;
        glm_cross(edge,surface_normal,edge1_normal);
        glm_normalize(edge1_normal);
        
    }
        
    
    
}   