#ifndef EDITOR_MODE
#define EDITOR_MODE
#include "editor.h"
void upate_editor_modes();
void change_to_editor_sub_mode(EditorMode mode);
void change_to_editor_mode(EditorMode mode);

EditorMode mode_to_change;
#endif