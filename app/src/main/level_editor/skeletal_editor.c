#include "skeletal_editor.h"

struct Geometry skeletal_bones_gizmo_geometry;
GLuint vertex_buffer_id;


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
    
    glGenBuffers(1,&vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, skeletal_bones_gizmo_geometry.vertex_count * sizeof(struct Vertex) , skeletal_bones_gizmo_geometry.vertices, GL_STATIC_DRAW);
}

void draw_skeletal_bones(){

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(struct Vertex),(void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glDrawArrays(GL_POINTS, 0, skeletal_bones_gizmo_geometry.vertex_count);
    glLineWidth(3);

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
}

void clean_skeletal_editor(){
    if(skeletal_bones_gizmo_geometry.vertices != NULL){

    }
}
