//
// Created by pavon on 6/24/19.
//

#include "object_select.h"
#include "game.h"

void init_menu(){


    array_init(&menu_select.buttons, sizeof(Button), 10);
    array_add(&menu_select.buttons, &next_item_button);
    array_add(&menu_select.buttons, &previous_item_button);



}

void update_menu(){
    if(next_item_button.pressed){

    }else if(previous_item_button.pressed){

    }
}