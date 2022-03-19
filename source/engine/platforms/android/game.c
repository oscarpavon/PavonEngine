#include "game.h"
#include <engine/engine.h>
#include <engine/game.h>

void lala_init(){


}

void lala_end(){

}

void lala_loop(){


}

void game_init(){

  PGame game;
  ZERO(game);
  game.init = &lala_init;
  game.loop = &lala_loop;
  game.end = &lala_end;
  game.name = "Test";
  pe_game_create(&game);


}
