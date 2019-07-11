//
// Created by pavon on 6/24/19.
//

#ifndef PAVON_MOBILE_GAME_H
#define PAVON_MOBILE_GAME_H

#include "model.h"
#include "level.h"



struct Player{
    struct Model* model;

};
Level new_level;
struct Player player;

void init_game();

void update_game();

void update_viewport_size();

#endif //PAVON_MOBILE_GAME_H
