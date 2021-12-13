#include "chess.h"


void chess_loop(){

  vec3 move2 = {0,0.01,0};
  update_translation(move2);
}

void chess_init(){
    
  add_element_with_model_path("/home/pavon/chess/rook.glb");
  vec3 move = {1,1,0};
  update_translation(move);
  add_element_with_model_path("/home/pavon/chess/pawn.glb");
  vec3 move2 = {3,4,0};
  update_translation(move2);

  camera_init(&main_camera); 
  init_vec3(-10,0,3, main_camera.position);
  camera_update(&main_camera);
  
  
 


}


int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    chess.init = &chess_init;
    pe_game_create(&chess);
}
