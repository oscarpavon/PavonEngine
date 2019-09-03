#ifndef EDITOR_CONTENT_MANAGER_H
#define EDITOR_CONTENT_MANAGER_H
#include <dirent.h>
#include "../Engine/numbers.h"
#include "../Engine/content_manager.h"


void content_manager_import(const char* path);
void content_manager_init();
static u32 content_GUID_count = 0;
#endif
