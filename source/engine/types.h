#ifndef ENGINE_DATA_TYPES
#define ENGINE_DATA_TYPES

#include <stdbool.h>

#include "array.h"
#include "numbers.h"
#include "../ThirdParty/cglm/types.h"

#ifdef EDITOR
    #include "../editor/types.h"
#endif 
#ifdef ANDROID    
    struct android_app* app;
#endif


typedef struct TransformComponent TransformComponent;
typedef struct ComponentDefinition ComponentDefinition;

typedef struct Element{
    unsigned int id;
    char name[20];
    bool proccess;
    bool selected;
    Array components;
    TransformComponent* transform;
#ifdef EDITOR
    ElementEditorData editor_data;
#endif
}Element;

typedef struct PlayerStart{
    vec3 position;
    versor rotation; 
}PlayerStart;

typedef void(*Action)(void);

typedef struct ActionPointer{
    int id;
    Action action;
}ActionPointer;

typedef struct{
    void (*command)(void*);
	bool executed;
	void* parameter;
}ExecuteCommand;


#endif // !ENGINE_DATA_TYPES
