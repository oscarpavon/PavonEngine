#include "chess.h"

vec4 color1 = {0,0.2,0,1};
vec4 color2 = {1,0.5,1,1};

vec4 piece_color1 = {0.2,0.2,0,1};
vec4 piece_color2 = {0.5,0.5,1,1};

PMesh check_mesh;
PMaterial check_board_mat2;
PMaterial check_board_mat1;


PMaterial piece_mat2;
PMaterial piece_mat1;

void chess_piece_set_pos(vec2 pos){
  
  pe_element_set_position(selected_element,VEC3(pos[0],pos[1],0.3));

}

void chess_piece_init_scale(){
  float scale = -0.8f;
  pe_element_set_scale(VEC3(scale,scale,scale)) ;
}

void chess_pieces_create(){

  add_element_with_model_path("/home/pavon/chess/rook.glb");
  chess_piece_set_pos(VEC2(4,6)) ;

  chess_piece_init_scale();

  add_element_with_model_path("/home/pavon/chess/pawn.glb");
  chess_piece_set_pos(VEC2(2,0)) ;
  chess_piece_init_scale();

  add_element_with_model_path("/home/pavon/chess/king.glb");
  chess_piece_set_pos(VEC2(3,7)) ;
  chess_piece_init_scale();


  add_element_with_model_path("/home/pavon/chess/queen.glb");
  chess_piece_set_pos(VEC2(5,3)) ;
  chess_piece_init_scale();

}
void chess_move_piece(vec2 pos){
  
  float scale = 0.8f;
  pe_element_set_scale(VEC3(scale,scale,scale)) ;
  chess_piece_set_pos(pos);
  chess_piece_init_scale();

}

void chess_board_create() {

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      new_empty_element();

      add_transform_component_to_selected_element();

      pe_comp_static_mesh_add_to_element();

      pe_element_comp_init();

      StaticMeshComponent *mesh2 =
          get_component_from_element(selected_element, STATIC_MESH_COMPONENT);

        if ((x+y) % 2 == 0) {
          mesh2->material = check_board_mat2;

        } else {

          mesh2->material = check_board_mat1;
        }

      selected_model->mesh = check_mesh;

      pe_element_set_position(selected_element, VEC3(x, y, 0));
    }
  }
}
void chess_input(){
  if(key_released(&input.A)){
    LOG("a pressed\n");
    chess_move_piece(VEC2(7,7))  ;
   }    
  
}

void chess_init(){

  camera_init(&main_camera); 
  init_vec3(-7,3.5,3.6, main_camera.position);
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
  
  chess_pieces_create();
}

void chess_loop(){


}


int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    chess.init = &chess_init;
    chess.input = &chess_input;
    pe_game_create(&chess);
}
