#include "../../Engine/engine.h"
#include "collision.h"
#include "../../Engine/vertex.h"

void collision_SAT_project_on_axis(Model* model, vec3 axis, float* projection){
    //project on axis
     u8 vertex_count = model->vertex_array.count;

     Vertex* vertex = array_get(&model->vertex_array,0);
       vec3 global_position;
        mat4 position;
        glm_mat4_identity(position);
        glm_translate(position,vertex->postion);
        glm_translate(position,model->model_mat[3]);

        glm_vec3_copy(position[3],global_position);


       float point = glm_vec3_dot(axis,global_position);

    float max = point;
    float min = max;

    for(u8 i = 1; i < vertex_count ; i++){
       Vertex* vertex = array_get(&model->vertex_array,i);

       vec3 global_position;
        mat4 position;
        glm_mat4_identity(position);
        glm_translate(position,vertex->postion);
        glm_translate(position,model->model_mat[3]);

        glm_vec3_copy(position[3],global_position);

 
       float point = glm_vec3_dot(axis,global_position);
        if(point < min){
            min = point;
        }else if (point > max) {
            max = point;
        }
    }

    projection[1] = min;
    projection[0] = max;
}

void get_edges_axis(Model* model, vec3* axis){
    u8 vertex_count = model->vertex_array.count;
       
    for (u8 i = 0; i < vertex_count; i+=3)
    {
        

        Vertex* vertex = array_get(&model->vertex_array,i);
        Vertex* vertex2 = array_get(&model->vertex_array,i+1);
        Vertex* vertex3 = array_get(&model->vertex_array,i+2);


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
        
        glm_vec3_copy(edge1_normal,&axis[i][0]);
        glm_vec3_copy(edge1_normal,&axis[i+1][0]);
        glm_vec3_copy(edge1_normal,&axis[i+2][0]);

    } 

}   


bool collision_of(Model* model, Model* model2){
    u8 vertex_count = model->vertex_array.count;
    u8 vertex_count2 = model2->vertex_array.count;

    vec3 axis[vertex_count];
    memset(axis,0,sizeof(axis));
    vec3 axis2[vertex_count2];
    memset(axis2,0,sizeof(axis2));
    get_edges_axis(model,axis);
    get_edges_axis(model2,axis2);

    for (u8 i = 0; i < vertex_count; i++)
    {
        vec2 projection;
        vec2 projection2;
        collision_SAT_project_on_axis(model,axis[i],projection);
        collision_SAT_project_on_axis(model2,axis[i],projection2);

        if(projection[0] < projection2[1] || projection2[0] < projection[1])
            return false;
    }
    
    for (u8 i = 0; i < vertex_count2; i++)
    {
        vec2 projection;
        vec2 projection2;
        collision_SAT_project_on_axis(model,axis2[i],projection);
        collision_SAT_project_on_axis(model2,axis2[i],projection2);

        if(projection[0] < projection2[1] || projection2[0] < projection[1])
            return false;
    }

    return true;
}
