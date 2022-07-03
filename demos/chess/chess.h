#ifndef CHESS
#define CHESS

#include <engine/engine.h>
#include <engine/game.h>

typedef struct ChessPiece{
  Element* element;
  
}ChessPiece;
  
typedef struct ChessMovement{
  ChessPiece* piece;
  char movemenet[10];
}ChessMovement;

void chess_init();
void chess_loop();
void chess_draw();
void chess_input();

#endif
