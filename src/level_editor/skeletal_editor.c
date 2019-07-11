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

    skeletal_bones_gizmo_geometry.vertices = malloc(sizeof(vertices));
    memcpy(skeletal_bones_gizmo_geometry.vertices, vertices, sizeof(vertices));
    skeletal_bones_gizmo_geometry.vertex_count = vertex_count;

}

void init_skeletal_gizmo_geometry(){
    
    glGenBuffers(1,&skeletal_gizmo_vertices_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,skeletal_gizmo_vertices_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, skeletal_bones_gizmo_geometry.vertex_count * sizeof(struct Vertex) , skeletal_bones_gizmo_geometry.vertices, GL_STATIC_DRAW);
}


void init_skeletal_gizmo(){
    Skeletal* skeletal = selected_element->model->skeletal;

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
    skeletal_bones_gizmo_geometry.vertices = malloc(sizeof(vertices));
    memcpy(skeletal_bones_gizmo_geometry.vertices, vertices, sizeof(vertices));
    skeletal_bones_gizmo_geometry.vertex_count = vertex_count;

    //create_skeletal_vertices_bones_gizmo();
    init_skeletal_gizmo_geometry();
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
   
	glDrawArrays(GL_POINTS, 0, skeletal_bones_gizmo_geometry.vertex_count);
    

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        LOGW("skeletal draw error \n");
        LOGW("Error %08x \n",error);
    }

}


void init_skeletal_editor(){
    skeletal_bones_gizmo_geometry.vertices = NULL;
    create_skeletal_vertices_bones_gizmo();
    init_skeletal_gizmo_geometry();

    skeletal_blue_shader = compile_shader(skeletal_blue_joint_source,GL_FRAGMENT_SHADER);

    skelta_gizmo_shader = glCreateProgram();
    
    glAttachShader(skelta_gizmo_shader, standart_vertex_shader);
    glAttachShader(skelta_gizmo_shader, skeletal_blue_shader);
    glLinkProgram(skelta_gizmo_shader);
}

void clean_skeletal_editor(){
    if(skeletal_bones_gizmo_geometry.vertices != NULL){

    }
}
