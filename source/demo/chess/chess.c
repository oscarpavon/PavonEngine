#include "chess.h"


void chess_pieces_load(){

  add_element_with_model_path("/home/pavon/chess/rook.glb");
  vec3 move = {0,4,0};
  pe_element_set_position(selected_element,move);

  add_element_with_model_path("/home/pavon/chess/pawn.glb");

  vec3 pawn = {0,0,0};
  pe_element_set_position(selected_element,pawn);

}

void chess_init(){
    
  add_element_with_model_path("/home/pavon/chess/check.glb");

  vec3 checkpos = {0,0,0};
  pe_element_set_position(selected_element,checkpos);



  camera_init(&main_camera); 
  init_vec3(-10,0,3, main_camera.position);
  camera_update(&main_camera);
  
  PMaterial check_board_mat1;
  ZERO(check_board_mat1);
  check_board_mat1.shader = shader_standard_color;
  vec4 color1 = {0,1,0,1};
  glm_vec4_copy(color1,check_board_mat1.color) ;


  PMaterial check_board_mat2;
  ZERO(check_board_mat2);
  check_board_mat2.shader = shader_standard_color;
  vec4 color2 = {0,1,0,1};
  glm_vec4_copy(color2,check_board_mat2.color) ;

  StaticMeshComponent* mesh = get_component_from_element(selected_element,STATIC_MESH_COMPONENT);
  mesh->material = check_board_mat1;

  

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
