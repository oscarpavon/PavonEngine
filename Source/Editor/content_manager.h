#ifndef EDITOR_CONTENT_MANAGER_H
#define EDITOR_CONTENT_MANAGER_H
#include <dirent.h>
#include "../Engine/numbers.h"
typedef enum ContentType{
    CONTENT_TYPE_STATIC_MESH,
    CONTENT_TYPE_SKINNED_MESH,
    CONTENT_TYPE_TEXTURE,
    CONTENT_TYPE_SOUND,
    CONTENT_TYPE_SHADER,
    CONTENT_TYPE_MATERIAL
}ContentType;

typedef struct Content{
    char name[40];
    char path[100];
    ContentType type;
    u32 id;
}Content;

void content_manager_init();

#endif