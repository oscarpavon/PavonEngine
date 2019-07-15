#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

typedef void(*SaveDataFunction)(int id_data);

void save_level_data(const char*);
void save_gui_data(const char* gui_name);

#endif