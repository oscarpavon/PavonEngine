#include "chess.h"


void chess_pieces_load(){

  add_element_with_model_path("/home/pavon/chess/rook.glb");
  vec3 move = {0,4,0};
  pe_element_set_position(selected_element,move);

  add_element_with_model_path("/home/pavon/chess/pawn.glb");

  vec3 pawn = {0,0,0};
  pe_element_set_position(selected_element,pawn);

}

vec4 color1 = {0,0,0,1};
vec4 color2 = {1,1,1,1};

PMesh check_mesh;
PMaterial check_board_mat2;
PMaterial check_board_mat1;

void chess_board_create() {
  int x, y = 0;
  int offset = 2;
  for (int i = 0; i < 8; i++) {
    for (int o = 0; o < 8; o++) {
      new_empty_element();

      add_transform_component_to_selected_element();

      pe_comp_static_mesh_add_to_element();

      pe_element_comp_init();

      StaticMeshComponent *mesh2 =
          get_component_from_element(selected_element, STATIC_MESH_COMPONENT);

        if ((i+o) % 2 == 0) {

          mesh2->material = check_board_mat2;
        } else {

          mesh2->material = check_board_mat1;
        }

      selected_model->mesh = check_mesh;

      pe_element_set_position(selected_element, VEC3(i, o, 0));
    }
  }
}

void chess_init(){

  camera_init(&main_camera); 
  init_vec3(-17,0,3, main_camera.position);
  camera_update(&main_camera);
  
  ZERO(check_board_mat1);
  check_board_mat1.shader = shader_standard_color;
  glm_vec4_copy(color1,check_board_mat1.color) ;


  ZERO(check_board_mat2);
  check_board_mat2.shader = shader_standard_color;
  glm_vec4_copy(color2,check_board_mat2.color) ;

  add_element_with_model_path("/home/pavon/chess/check.glb");

  vec3 checkpos = {0,0,29};
  pe_element_set_position(selected_element,checkpos);

  StaticMeshComponent* mesh = get_component_from_element(selected_element,STATIC_MESH_COMPONENT);
  mesh->material = check_board_mat1;
  ZERO(check_mesh);

  Model* m = array_get_pointer(&mesh->models_p,0);
  check_mesh = m->mesh;
  
  chess_board_create();

}


void chess_loop(){


}


int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    chess.init = &chess_init;
    pe_game_create(&chess);
}
