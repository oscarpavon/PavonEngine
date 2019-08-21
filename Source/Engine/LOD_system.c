#include "LOD_system.h"
#include "../engine.h"
void LOD_system_calculate_AABB_on_screen(StaticMeshComponent* mesh_component, vec2 min_max[2]){
    mat4 projection_view;
    glm_mul(main_camera.projection,main_camera.view,projection_view);
    mat4 min_mat;
    glm_translate(min_mat,&mesh_component->center[0]);
    glm_mul(projection_view,min_mat, min_mat);
    mat4 max_mat;
    glm_translate(min_mat,&mesh_component->center[1]);
    glm_mul(projection_view,max_mat, max_mat);

    min_mat[3][0] = ( (min_mat[3][0] + 1.f) / 2.f ) * camera_width_screen;
    max_mat[3][0] = ( (max_mat[3][0] + 1.f) / 2.f ) * camera_width_screen;
    
    
    min_mat[3][1] = ( (1.f - min_mat[3][1]) / 2.f ) * camera_heigth_screen;
    max_mat[3][1] = ( (1.f - max_mat[3][1]) / 2.f ) * camera_heigth_screen;


    vec2 min_on_screen;
    min_on_screen[0] = min_mat[3][0];
    min_on_screen[1] = min_mat[3][1];

    vec2 max_on_screen;
    max_on_screen[0] = max_mat[3][0];
    max_on_screen[1] = max_mat[3][1];
    
}

void LOD_check_distance_static_mesh_component_and_add_to_draw_elements(StaticMeshComponent* static_mesh_component){
    float distance = glm_vec3_distance(main_camera.position,static_mesh_component->center);
    
    float distaces[3] = {0,24,40};

    u8 id;
    int count = static_mesh_component->meshes.count-1;
    for(int i = 1; i <= count ; i++){
        float distance_value = distaces[i-1];            
        if(distance_value==0){
            u8* model_id = array_get(&static_mesh_component->meshes,i);
            id = *model_id;
            continue;
        }
        float next_distace_value = distance_value+1;
        if(i != count)
            next_distace_value = distaces[i];
            
        if(distance >= distance_value && distance_value < next_distace_value){
            u8* model_id = array_get(&static_mesh_component->meshes,i);
            id = *model_id;            
        }
        
    }
    Model* draw_model = array_get(actual_model_array,id);
    //Model* other = array_get(actual_model_array,1);
    
    array_add(&frame_draw_elements,&draw_model);
}  