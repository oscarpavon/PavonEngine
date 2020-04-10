
#ifndef EDITOR_LEVEL_H
#define EDITOR_LEVEL_H

void editor_level_new(const char * path);
void editor_level_open(const char* path);
void editor_level_save(const char* path);

char editor_level_open_path[200];
#endif
