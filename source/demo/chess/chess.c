#include "chess.h"



void chess_init(){
    
  add_element_with_model_path("/home/pavon/chess/rook.glb");
  vec3 move = {0,4,0};
  pe_element_set_position(selected_element,move);

  add_element_with_model_path("/home/pavon/chess/pawn.glb");

  vec3 pawn = {0,0,0};
  pe_element_set_position(selected_element,pawn);

  camera_init(&main_camera); 
  init_vec3(-10,0,3, main_camera.position);
  camera_update(&main_camera);
  

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
