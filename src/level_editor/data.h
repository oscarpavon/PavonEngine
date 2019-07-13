#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

typedef void(*SaveDataFunction)(void);

void save_level_data(const char*);
void save_gui_data(const char* gui_name);

static const char* const level_folder = "../Game/levels/";
static const char* const gui_folder = "../assets/gui/";


#endif