
#include <engine/utils.h>
#include <engine/log.h>
#include <engine/vertex.h>

#include <engine/renderer/renderer.h>
#include <engine/renderer/opengl/opengl_es2.h>

GLuint new_mesh_vertex_buffer; 

Array first_model;


void init_modeling(){


}

void draw_vertices(){
	
    mat4 model;
    glm_mat4_identity(model);
		mat4 mvp;
    update_mvp(model, mvp);  


   // update_draw_vertices(shader,new_mesh_vertex_buffer,mvp);  

		glDrawArrays(GL_POINTS, 0, first_model.count);


}

void vertex_new_on_array(Array* array, vec3 pos){
  
  Vertex new_vertex;
  ZERO(new_vertex);
  
  glm_vec3_copy(pos, new_vertex.postion);

  array_add(array,&new_vertex);

   
}

void vertex_create(){

  LOG("Vertex created");
  
    

  vertex_new_on_array(&first_model,(vec3){0,0,0});



}


void geometry_create_vertex_buffer(){

    GLuint buffer; 
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
//    glBufferData(GL_ARRAY_BUFFER, vertex_array->count * sizeof(struct Vertex) , vertex_array->data, GL_STATIC_DRAW);
}


