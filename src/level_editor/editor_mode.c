#include "editor_mode.h"

#include "../game.h"
#include "../camera.h"

bool game_initialized = false;

void change_to_editor_sub_mode(EditorMode mode){    
       
    switch (mode)
    { 
        case EDITOR_SUB_MODE_TEXT_INPUT:
            editor_sub_mode_text = "Text Input";
            editor_sub_mode = mode;
            break;
        case EDITOR_SUB_MODE_NULL:
            editor_sub_mode_text = "";
            editor_sub_mode = mode;
            break;
    }
}
void change_to_editor_mode(EditorMode mode){
    if(editor_mode == mode){
        editor_mode = EDITOR_CHANGING_MODE_MODE;
        change_to_editor_mode(EDITOR_DEFAULT_MODE);
        return;
    }
    editor_mode = EDITOR_CHANGING_MODE_MODE;
    mode_to_change = mode;
    switch (mode)
    {    
    case EDITOR_DEFAULT_MODE:
        editor_mode_show_text = "Default Mode";
        break;
    case EDITOR_NAVIGATE_MODE:
        editor_mode_show_text = "Navigate Mode";
        move_camera_input = true;
        break;
    case EDITOR_GRAB_MODE:
        editor_mode_show_text = "Grab Mode";
        break;
    case EDITOR_ROTATE_MODE:
        editor_mode_show_text = "Rotate Mode";
        break;
    case EDITOR_MODE_GUI_EDITOR:
        editor_mode_show_text = "GUI Editor";
        break;
    case EDITOR_PLAY_MODE:
        if(game_initialized == false)
            init_game();
        game_initialized = true;
        editor_mode_show_text = "Play Mode";
        break;
    case EDITOR_CHANGING_MODE_MODE:
        editor_mode = mode_to_change;
        break;    
    default:
        break;
    } 
    memset(&input,0,sizeof(Input));
}