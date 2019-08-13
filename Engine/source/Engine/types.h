#ifndef ENGINE_DATA_TYPES
#define ENGINE_DATA_TYPES

#ifdef EDITOR
    #include "../Editor/types.h"
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


typedef struct Sphere{
    vec3 center;
    float radius;
    float volume;
}Sphere;

typedef struct PlayerStart{
    vec3 position;
    versor rotation; 
}PlayerStart;

typedef void(*Action)(void);

typedef struct ActionPointer{
    int id;
    Action action;
}ActionPointer;

#endif // !ENGINE_DATA_TYPES
