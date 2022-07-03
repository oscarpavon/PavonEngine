
#include <engine/utils.h>
#include <engine/log.h>
#include <engine/vertex.h>

#include <engine/renderer/renderer.h>
#include <engine/renderer/opengl/opengl_es2.h>
#include <engine/engine.h>
#include <engine/components/static_mesh_component.h>

GLuint new_mesh_vertex_buffer; 


Model model;

Array vertex_modeling;

uint current_select_vertex;

void pe_modeling_update_vertex_selected(Model* model){
  for(int i = 0; i < model->vertex_array.count ; i++){
    Vertex* vertex = array_get(&model->vertex_array, i);
    if(vertex->selected == true){
      glm_vec3_copy(VEC3(0,1,0), vertex->color);
    }else{
      glm_vec3_copy(VEC3(0,0,1), vertex->color);
    }
  }
}

Vertex* pe_modeling_get_vertex_by_id(Array* array , u64 id){
  Vertex* out = NULL; 
  for(u64 i = 0; i < array->count; i++) {
      Vertex* vertex = array_get(array, i);
      if(vertex->id == id){
        return vertex;
      }
  }
  LOG("## Vertex not found");
}
void pe_modeling_vertex_update(){

  glBindBuffer(GL_ARRAY_BUFFER, new_mesh_vertex_buffer);

  glBufferData(GL_ARRAY_BUFFER, model.vertex_array.count * sizeof(Vertex),
               model.vertex_array.data, GL_DYNAMIC_DRAW);
}

void pe_modeling_select_next_vertex(){
  current_select_vertex++;
  Vertex* vertex = pe_modeling_get_vertex_by_id(&model.vertex_array, current_select_vertex);
  vertex->selected = true;
  pe_modeling_update_vertex_selected(&model); 
  pe_modeling_vertex_update();
}

void draw_vertices(){
	
    mat4 model_mat;
    glm_mat4_identity(model_mat);
		mat4 mvp;
    update_mvp(model_mat, mvp);  

    glUseProgram(model.shader);   

    glBindBuffer(GL_ARRAY_BUFFER, new_mesh_vertex_buffer);

    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,color));

    GLint mvp_uniform =  get_uniform_location(model.shader,"MVP");

    send_color_to_shader(model.shader, VEC4(0,0,1,1));

    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, mvp);
		glDrawArrays(GL_POINTS, 0, model.vertex_array.count);
    

}


void vertex_new(float x , float y , float z){

  Vertex new_vertex;
 
  ZERO(new_vertex);

  new_vertex.postion[0] = x;
  new_vertex.postion[1] = y;
  new_vertex.postion[2] = z;

  new_vertex.selected = false;  


  array_add(&model.vertex_array, &new_vertex);
   
  pe_modeling_update_vertex_selected(&model) ;
  pe_modeling_vertex_update();

  LOG("## Vertex Added");
}

void init_modeling() {
  ZERO(model);

  array_init(&model.vertex_array, sizeof(Vertex), 100);

  glGenBuffers(1, &new_mesh_vertex_buffer);

  //pe_comp_static_mesh_shader_init(&model);
  
  GLuint frag = pe_shader_load_src_and_create("/sdcard/Download/chess/vertex_modelling.frag", GL_FRAGMENT_SHADER);
  

  pe_shader_create_for_model(&model, frag, standart_vertex_shader);

  current_select_vertex = -1;

}
