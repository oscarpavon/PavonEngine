#ifndef EDITOR_MODE
#define EDITOR_MODE
#include "editor.h"

typedef enum EditorMode{
    EDITOR_GRAB_MODE,
    EDITOR_DEFAULT_MODE,
    EDITOR_SCALE_MODE,
    EDITOR_ROTATE_MODE,
    EDITOR_NAVIGATE_MODE,
    EDITOR_CHANGING_MODE_MODE,
    EDITOR_PLAY_MODE,
    EDITOR_MODE_GUI_EDITOR,
    EDITOR_SUB_MODE_TEXT_INPUT,
    EDITOR_SUB_MODE_NULL,
    EDITOR_SUB_MODE_GRAB,
    EDITOR_SUB_MODE_SCALE,
    EDITOR_SUB_MODE_ROTATE }EditorMode;

void upate_editor_modes();
void change_to_editor_sub_mode(EditorMode mode);
void change_to_editor_mode(EditorMode mode);


EditorMode mode_to_change;

enum EditorMode editor_mode;
enum EditorMode editor_sub_mode;

#define EDITOR_CONSOLE_MODE_NORMAL 1


#endif
