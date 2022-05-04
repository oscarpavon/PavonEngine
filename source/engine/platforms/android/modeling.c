
#include <engine/utils.h>
#include <engine/log.h>
#include <engine/vertex.h>

#include <engine/renderer/renderer.h>
#include <engine/renderer/opengl/opengl_es2.h>
#include <engine/engine.h>

GLuint new_mesh_vertex_buffer; 


Model model;

int vertex_count;

void draw_vertices(){
	
    mat4 model_mat;
    glm_mat4_identity(model_mat);
		mat4 mvp;
    update_mvp(model_mat, mvp);  


    update_draw_vertices(model.shader,new_mesh_vertex_buffer,mvp);  

		glDrawArrays(GL_POINTS, 0, vertex_count);
    

}

void vertex_new_on_array(Array* array, vec3 pos){
  
  Vertex new_vertex;
  ZERO(new_vertex);
  
  glm_vec3_copy(pos, new_vertex.postion);

  array_add(array,&new_vertex);

   
  glBindBuffer(GL_ARRAY_BUFFER,new_mesh_vertex_buffer);


  glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_count, sizeof(Vertex) ,&new_vertex);
  vertex_count++;
}

void vertex_new(float x , float y , float z){
  //vertex_new_on_array(&model.vertex_array,VEC3(x,y,z));

  Vertex new_vertex;
 
  ZERO(new_vertex);

  new_vertex.postion[0] = -30;
  new_vertex.postion[1] = -30;
  new_vertex.postion[2] = -30;

  

   
  glBindBuffer(GL_ARRAY_BUFFER, new_mesh_vertex_buffer);


  glBufferSubData(GL_ARRAY_BUFFER, 0 , sizeof(Vertex) ,&new_vertex);
  vertex_count++;


}

void vertex_create(){

  LOG("Vertex created");

}

void init_modeling() {
  vertex_count = 0;
  ZERO(model);

  array_init(&model.vertex_array, sizeof(Vertex), 100);

  //  vertex_new_on_array(&model.vertex_array,(vec3){0,0,0});

  glGenBuffers(1, &new_mesh_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, new_mesh_vertex_buffer);

  Vertex v[3];
  //ZERO(v);

  v[0].postion[0] = 0;
  v[0].postion[1] = 0;
  v[0].postion[2] = 0;


  v[1].postion[0] = -20;
  v[1].postion[1] = -1;
  v[1].postion[2] = -1;


  v[2].postion[0] = -1;
  v[2].postion[1] = 0;
  v[2].postion[2] = 1;

  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), v,
               GL_STATIC_DRAW);

  pe_comp_static_mesh_shader_init(&model);
}
