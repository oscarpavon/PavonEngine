//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_GAME_H
#define PAVON_MOBILE_GAME_H

#include "model.h"

int init_game();

void update_game();

int load_gamplay_code();

bool loaded_gamplay_library;
void* dynamic_lib_handle;
void(*loop_fuction_dynamic_loaded)(void); 
void close_dynamic_game_play();
#endif //PAVON_MOBILE_GAME_H
