#include "chess.h"


void chess_loop(){

}

int main(){
    PGame chess;
    ZERO(chess);
    chess.name = "Chess";
    chess.loop = &chess_loop;
    pe_game_create(&chess);
}
