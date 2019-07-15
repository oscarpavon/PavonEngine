#include "skeletal_editor.h"
#include "../shader.h"
#include "../engine.h"
#include "editor.h"

struct Geometry skeletal_bones_gizmo_geometry;
GLuint skeletal_gizmo_vertices_buffer_id;


void create_skeletal_vertices_bones_gizmo(){
    
    
    int vertex_count = 1;
    struct Vertex vertices[vertex_count];
    memset(vertices,0,sizeof(vertices));

    struct Vertex vert = {{0,0,0},{0,0}};
    memcpy(vertices,&vert,sizeof(struct Vertex));

    add_vextex_to_array(&skeletal_bones_gizmo_geometry.vertex_array,vertices[0]);
    
}

void init_skeletal_gizmo(){
    Skeletal* skeletal = selected_element->model->skeletal;
    if(skeletal == NULL)
        return;
    int vertex_count = skeletal->joints_count;
    struct Vertex vertices[vertex_count];
    memset(vertices,0,sizeof(vertices));

    for(int i = 0; i < skeletal->joints_count ; i++){
        mat4 local;        
        get_global_matrix(&skeletal->joints[i], local);
        glm_mat4_mul(selected_element->model->model_mat, local, local);
        struct Vertex vert = { { local[3][0],local[3][1],local[3][2] } ,{0,0}};
        //struct Vertex vert = { { skeletal->joints[i].translation[0] ,skeletal->joints[i].translation[1] ,skeletal->joints[i].translation[2]} ,{0,0}};
        memcpy(&vertices[i],&vert,sizeof(struct Vertex));

    }
   
    for(int i = 0; i < vertex_count ; i++){
        add_vextex_to_array(&skeletal_bones_gizmo_geometry.vertex_array,vertices[i]);
    }
 
}

GLuint skelta_gizmo_shader;

void draw_skeletal_bones(){
    glBindBuffer(GL_ARRAY_BUFFER, skeletal_gizmo_vertices_buffer_id);

    glUseProgram(skelta_gizmo_shader);

    mat4 mvp;
    glm_mat4_identity(mvp);

    mat4 model;
    glm_mat4_identity(model);
    update_mvp(model, mvp);

    GLint mvp_uniform =  glGetUniformLocation(skelta_gizmo_shader,"MVP");

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);
    
    glLineWidth(10);
   
	glDrawArrays(GL_POINTS, 0, skeletal_bones_gizmo_geometry.vertex_array.count);
    

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("skeletal draw error \n");
        LOGW("Error %08x \n",error);
    }

}


void init_skeletal_editor(){
    init_vertex_array(&skeletal_bones_gizmo_geometry.vertex_array,1);
    create_skeletal_vertices_bones_gizmo();
    init_static_gpu_vertex_buffer(&skeletal_bones_gizmo_geometry.vertex_array,&skeletal_gizmo_vertices_buffer_id);

    skeletal_blue_shader = compile_shader(skeletal_blue_joint_source,GL_FRAGMENT_SHADER);

    skelta_gizmo_shader = create_engine_shader(standart_vertex_shader,skeletal_blue_shader); 
    
}

void clean_skeletal_editor(){
   
}
