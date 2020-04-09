#include "skeletal_editor.h"
#include "../engine/shader.h"
#include "../engine/engine.h"
#include "editor.h"

#include "../engine/components/components.h"
#include "../engine/vertex.h"

struct LoadGeometry skeletal_bones_gizmo_geometry;
GLuint skeletal_gizmo_vertices_buffer_id;
GLuint skeletal_gizmo_index_buffer_id;

void assign_nodes_indices(Skeletal* skeletal){
    int vertex_count = skeletal->joints_count;
    int index = 0;
    for(int i = 0; i < skeletal->joints_count ; i++){
        skeletal->joints[i].id = index;       
        index++;
    }
}

void update_joints_vertex(){  

    update_gpu_vertex_data(&skeletal_bones_gizmo_geometry.vertex_array, skeletal_gizmo_vertices_buffer_id);
}

void clear_skeletal_vertices(){
    array_clean(&skeletal_bones_gizmo_geometry.vertex_array);
}

void init_skeletal_vertices(mat4 global, int i, Node* current_joint){
    struct Vertex vert = { { global[3][0],global[3][1],global[3][2] } ,{0,0}};
    array_add(&skeletal_bones_gizmo_geometry.vertex_array,&vert);

    if(current_joint->parent != NULL){
        if(i == 2){
            int id = i-1;
            array_add(&skeletal_bones_gizmo_geometry.index_array,&id);
        }else if(i >= 3){
            array_add(&skeletal_bones_gizmo_geometry.index_array,&current_joint->parent->id);
        }
    }
    array_add(&skeletal_bones_gizmo_geometry.index_array,&i);

    LOG("Created vertices for: %s\n",current_joint->name);

}
void update_skeletal_vertices_gizmo(mat4 global, int i, Node* current_joint){
    struct Vertex vert = { { global[3][0],global[3][1],global[3][2] } ,{0,0}};
    if(skeletal_bones_gizmo_geometry.vertex_array.initialized)
    array_add(&skeletal_bones_gizmo_geometry.vertex_array,&vert);
}

void create_skeletal_vertices(){
    SkinnedMeshComponent* skin_component = get_component_from_selected_element(COMPONENT_SKINNED_MESH);
    if(!skin_component){
        LOG("No skinned mesh component\n");
        return;
    }

    Skeletal new_skeletal;
    memset(&new_skeletal,0,sizeof(Skeletal));
    new_skeletal.joints = array_get(&skin_component->joints,2);
    new_skeletal.joints_count = skin_component->joints.count-2;
    Skeletal* skeletal = &new_skeletal;
    assign_nodes_indices(skeletal);
    if(skeletal == NULL)
        return;
    int vertex_count = skeletal->joints_count;
    struct Vertex vertices[vertex_count];
    memset(vertices,0,sizeof(vertices));

    array_init(&skeletal_bones_gizmo_geometry.index_array,sizeof(unsigned short int),50);
    array_init(&skeletal_bones_gizmo_geometry.vertex_array,sizeof(Vertex),skeletal->joints_count);

    for(int i = 0; i < skeletal->joints_count ; i++){
        mat4 local;
        Node* current_joint = &skeletal->joints[i];       
        get_global_matrix(current_joint, local);
        mat4 global;
        glm_mat4_mul(selected_element->transform->model_matrix, local, global);
        
        init_skeletal_vertices(global,i,current_joint);
    }     

}

GLuint skelta_gizmo_shader;

void draw_skeletal_bones(){
    mat4 model;
    glm_mat4_identity(model);
    update_draw_vertices(skelta_gizmo_shader,skeletal_gizmo_vertices_buffer_id,model);  

	glDrawArrays(GL_POINTS, 0, skeletal_bones_gizmo_geometry.vertex_array.count);

    glLineWidth(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,skeletal_gizmo_index_buffer_id);
    glDrawElements(GL_LINES,skeletal_bones_gizmo_geometry.index_array.count,GL_UNSIGNED_SHORT,(void*)0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOG("skeletal draw error \n");
        LOG("Error %08x \n",error);
    }

}


void init_skeletal_editor(){      

    create_skeletal_vertices();
    init_static_gpu_vertex_buffer(&skeletal_bones_gizmo_geometry.vertex_array,&skeletal_gizmo_vertices_buffer_id);
    init_static_gpu_index_buffer(&skeletal_bones_gizmo_geometry.index_array,&skeletal_gizmo_index_buffer_id);  

    skeletal_blue_shader = compile_shader(skeletal_blue_joint_source,GL_FRAGMENT_SHADER);

    skelta_gizmo_shader = create_engine_shader(standart_vertex_shader,skeletal_blue_shader); 
    
}
