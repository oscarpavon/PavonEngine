#include "chess.h"


void chess_loop(){

}

void chess_init(){
    
    add_element_with_model_path("/home/pavon/chess/rook.glb");

}


int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    chess.init = &chess_init;
    pe_game_create(&chess);
}
